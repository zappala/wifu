#include "TCPPacketBuffer.h"

TCPPacketBuffer::TCPPacketBuffer() {

}

TCPPacketBuffer::~TCPPacketBuffer() {

}

int TCPPacketBuffer::insert(TCPPacket* p) {

    cout << "TCPPacketBuffer::insert(), Inserting seq num : " << p->get_tcp_sequence_number() << endl;
    cout << "TCPPacketBuffer::insert(), Inserting data len: " << p->get_data_length_bytes() << endl;

    if(!buffer_.empty()) {
        cout << cout << "TCPPacketBuffer::insert(), existing packets" << endl;
        for(packet_buffer::iterator i = buffer_.begin(); i != buffer_.end(); i++) {
            cout << "TCPPacketBuffer::insert(), existing seq num : " << i->first->get_tcp_sequence_number() << endl;
            cout << "TCPPacketBuffer::insert(), existing data len: " << i->first->get_data_length_bytes() << endl;
        }
    }





    pair < packet_buffer::iterator, bool> ret = buffer_.insert(make_pair(p, 0));

    int inserted_data_length = p->get_data_length_bytes();
    int num_bytes_inserted = inserted_data_length;
    u_int32_t inserted_sequence_number = p->get_tcp_sequence_number();

    if (!ret.second) {
        // the sequence number is already in the buffer
        TCPPacket* in_map = ret.first->first;

        if(in_map->get_data_length_bytes() < p->get_data_length_bytes()) {
            // the one in the map is smaller
            num_bytes_inserted -= in_map->get_data_length_bytes();
            buffer_.erase(ret.first);
            ret = buffer_.insert(make_pair(p, 0));
        }
        else {
            // p is smaller than the one in the map OR
            // they are equal in length
            // either way we can safely discard p
            return 0;
        }
    }
    list<packet_buffer::iterator> to_remove;

    // The iterator returned from the insert() call should point to the packet we just inserted
    // In order to figure out the number of bytes we should return, advance to the next packet (if any)
    packet_buffer::iterator itr = ret.first;
    itr++;
    while (itr != buffer_.end()) {
        TCPPacket* cur = itr->first;

        // check to see if we do not overlap
        if (less_than(inserted_sequence_number + inserted_data_length - 1, cur->get_tcp_sequence_number())) {
            break;
        }
        // we overlap
        // there are a few cases here
        // 1. we only overlap the very next packet, but not all of it
        // 2. we overlap the very next packet equally or more
        // 3. we overlap the very next packet and subsequent packets (taken care of with the loop)
        // 4. we overlap the very next packet, and possibly subsequent packets, but there is a gap after the next packet (taken care of with the loop)

        // 1. we only overlap the very next packet, but not all of it
        if (less_than(inserted_sequence_number + inserted_data_length,
                cur->get_tcp_sequence_number() + cur->get_data_length_bytes())) {
            num_bytes_inserted -= inserted_sequence_number + inserted_data_length - cur->get_tcp_sequence_number();
            break;
        }// 2. we overlap the very next packet equally or more
        else {
            num_bytes_inserted -= cur->get_data_length_bytes();
            to_remove.push_back(itr);
        }
        ++itr;
    }

    // we remove duplicate packets to make life easier on getting data out
    // we could probably make this a range, but this is likely easier to read
    // existing iterators are not invalidated by erasing other iterators
    while (!to_remove.empty()) {
        buffer_.erase(to_remove.front());
        to_remove.pop_front();
    }

    return num_bytes_inserted;
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
        else if (less_than(p->get_tcp_sequence_number(), sequence_number)) {
            int difference = sequence_number - p->get_tcp_sequence_number();
            int count = p->get_data_length_bytes() - difference;
            unsigned char* data = p->get_data() + difference;
            return_val.append((const char*) data, count);
            num_appended = count;
        }// gap
        else {
            break;
        }
        sequence_number += num_appended;
        ++itr;
    }

    buffer_.erase(buffer_.begin(), itr);
    return return_val;
}
