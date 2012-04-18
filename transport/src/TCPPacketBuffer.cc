#include "TCPPacketBuffer.h"

TCPPacketBuffer::TCPPacketBuffer() : started_(false) {
    mark_dirty();
}

TCPPacketBuffer::~TCPPacketBuffer() {

}

int TCPPacketBuffer::insert(TCPPacket* p) {
    assert(started_);

    int before_size = size();

    packet_buffer::iterator itr = lower_bound(buffer_.begin(), buffer_.end(), p, comparator_);
    TCPPacket* found_packet = *itr;

    bool already_inserted = itr != buffer_.end() && found_packet->get_tcp_sequence_number() == p->get_tcp_sequence_number();

    packet_buffer::iterator inserted = buffer_.insert(itr,p);
    mark_dirty();

    if(already_inserted) {
        if(found_packet->get_data_length_bytes() <= p->get_data_length_bytes()) {
            // keep the bigger of the two equal sequence numbered packets in the buffer
            buffer_.erase(itr);
        }
        else{
            buffer_.erase(inserted);
        }
    }

    int after_size = size();
    int total_inserted = after_size - before_size;

    assert(total_inserted >= 0);
    if(total_inserted == 0 && !already_inserted) {
        buffer_.erase(inserted);
    }
    return total_inserted;
}

TCPPacket* TCPPacketBuffer::remove(TCPPacket* p) {
    packet_buffer::iterator itr = lower_bound(buffer_.begin(), buffer_.end(), p, comparator_);
    TCPPacket* found_packet = *itr;
    if(found_packet == p) {
        buffer_.erase(itr);
        mark_dirty();
        return found_packet;
    }

    return 0;
    
}

void TCPPacketBuffer::get_continuous_data(u_int32_t sequence_number, gcstring& buffer) {
    assert(started_);
    
    packet_buffer::iterator itr = buffer_.begin();

    if(buffer_.empty()) {
        return;
    }

    assert(first_expected_sequence_number_ == sequence_number);
    

    while (itr != buffer_.end()) {
        TCPPacket* p = *itr;

        int num_appended = 0;

        // equal
        if (sequence_number == p->get_tcp_sequence_number()) {
            buffer.append((const char*) p->get_data(), p->get_data_length_bytes());
            num_appended = p->get_data_length_bytes();
        }// overlap
        else if (between(p->get_tcp_sequence_number(), sequence_number, p->get_tcp_sequence_number() + p->get_data_length_bytes())) {
            int difference = sequence_number - p->get_tcp_sequence_number();
            int count = p->get_data_length_bytes() - difference;
            unsigned char* data = p->get_data() + difference;
            buffer.append((const char*) data, count);
            num_appended = count;
        } // packet data has already been appended
        else if (less_than(p->get_tcp_sequence_number(), sequence_number)) {
            // do nothing
        }// gap
        else {
            break;
        }
        sequence_number += num_appended;
        ++itr;
    }

    first_expected_sequence_number_ = sequence_number;

    buffer_.erase(buffer_.begin(), itr);
    mark_dirty();
    return;
}

int TCPPacketBuffer::size() {
    assert(started_);

    if(size_ >= 0) {
        return size_;
    }

    size_ = 0;

    if(buffer_.empty()) {
        return size_;
    }

    packet_buffer::iterator itr = buffer_.begin();

    u_int32_t sequence_number = first_expected_sequence_number_;
    
    while (itr != buffer_.end()) {
        int count = 0;
        TCPPacket* p = *itr;

        // equal
        if (sequence_number == p->get_tcp_sequence_number()) {
            count += p->get_data_length_bytes();
        }// overlap
        else if (between(p->get_tcp_sequence_number(), sequence_number, p->get_tcp_sequence_number() + p->get_data_length_bytes())) {
            int difference = sequence_number - p->get_tcp_sequence_number();
            int length = p->get_data_length_bytes() - difference;
            count += length;
        } // packet has already been accounted for
        else if (less_than(p->get_tcp_sequence_number(), sequence_number)) {
            // do nothing
        }// gap
        else {
            count += p->get_data_length_bytes();
            sequence_number = p->get_tcp_sequence_number();
        }

        sequence_number += count;
        size_ += count;
        ++itr;
    }

    return size_;
}

void TCPPacketBuffer::mark_dirty() {
    size_ = -1;
}

u_int32_t TCPPacketBuffer::get_first_sequence_number() {
    return first_expected_sequence_number_;
}

void TCPPacketBuffer::set_first_sequence_number(u_int32_t seq_num) {
    first_expected_sequence_number_ = seq_num;
    started_ = true;
}