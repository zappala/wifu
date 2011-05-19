#include "TCPPacketBuffer.h"

TCPPacketBuffer::TCPPacketBuffer() {
    mark_dirty();
}

TCPPacketBuffer::~TCPPacketBuffer() {

}

int TCPPacketBuffer::insert(TCPPacket* p) {
    int before_size = size();

    pair < packet_buffer::iterator, bool> ret = buffer_.insert(make_pair(p, 0));
    mark_dirty();

    bool can_erase = ret.second;
    if(!ret.second) {
        TCPPacket* in_map = ret.first->first;
        // keep the bigger of the two equal sequence numbered packets in the buffer
        if(in_map->get_data_length_bytes() < p->get_data_length_bytes()) {
            buffer_.erase(ret.first);
            ret = buffer_.insert(make_pair(p,0));
            mark_dirty();
        }
    }

    int total_inserted = size() - before_size;

    assert(total_inserted >= 0);
    if(total_inserted == 0 && can_erase) {
        buffer_.erase(ret.first);
    }
    return total_inserted;
}

string TCPPacketBuffer::get_continuous_data(u_int32_t sequence_number) {

    packet_buffer::iterator itr = buffer_.begin();
    string return_val = "";

    if (buffer_.empty() || itr->first->get_tcp_sequence_number() != sequence_number) {
        return return_val;
    }

    while (itr != buffer_.end()) {
        TCPPacket* p = itr->first;

        int num_appended = 0;

        // equal
        if (sequence_number == p->get_tcp_sequence_number()) {
            return_val.append((const char*) p->get_data(), p->get_data_length_bytes());
            num_appended = p->get_data_length_bytes();
        }// overlap
        else if (between(p->get_tcp_sequence_number(), sequence_number, p->get_tcp_sequence_number() + p->get_data_length_bytes())) {
            int difference = sequence_number - p->get_tcp_sequence_number();
            int count = p->get_data_length_bytes() - difference;
            unsigned char* data = p->get_data() + difference;
            return_val.append((const char*) data, count);
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

    buffer_.erase(buffer_.begin(), itr);
    mark_dirty();
    return return_val;
}

int TCPPacketBuffer::size() {

    if(size_ >= 0) {
        return size_;
    }

    size_ = 0;

    if(buffer_.empty()) {
        return size_;
    }

    packet_buffer::iterator itr = buffer_.begin();

    u_int32_t sequence_number = itr->first->get_tcp_sequence_number();

    while (itr != buffer_.end()) {
        int count = 0;
        TCPPacket* p = itr->first;

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