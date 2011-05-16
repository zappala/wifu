#include "TCPPacketBuffer.h"
#include "Math.h"

TCPPacketBuffer::TCPPacketBuffer() {

}

TCPPacketBuffer::~TCPPacketBuffer() {

}

int TCPPacketBuffer::insert(TCPPacket* p) {
    int new_bytes = 0;

    list<TCPPacket*>::iterator itr = buffer_.begin();
    u_int32_t inserting_seq_num = p->get_tcp_sequence_number();
    u_int32_t inserting_length = p->get_data_length_bytes();


    while (itr != buffer_.end()) {
        TCPPacket* current = *itr;
        u_int32_t current_seq_num = current->get_tcp_sequence_number();


        if (inserting_seq_num == current_seq_num) {
            // no insertion on equal sequence numbers
            new_bytes = 0;
            break;
        }

        if (less_than(inserting_seq_num, current_seq_num)) {
            buffer_.insert(itr, p);

            // Figure out how much data was actually inserted
            if (less_than_or_equal(inserting_seq_num + inserting_length, current_seq_num)) {
                new_bytes = inserting_length;
            } else {
                new_bytes = current_seq_num - inserting_seq_num;
            }
            break;
        }
        ++itr;
    }

    if (itr == buffer_.end()) {
        buffer_.insert(buffer_.end(), p);
        new_bytes = inserting_length;
    }

    return inserting_length;
}

string TCPPacketBuffer::get_continuous_data(u_int32_t sequence_number) {

    string data = "";

    if(buffer_.empty()) {
        return data;
    }

    list<TCPPacket*>::iterator itr = buffer_.begin();
    TCPPacket* current = *itr;

    // no matching initial sequence number
    if (current->get_tcp_sequence_number() != sequence_number) {
        cout << "A" << endl;
        return data;
    }

    assert(itr == buffer_.begin());

    while (itr != buffer_.end()) {
        current = *itr;
        cout << "b" << endl;
        // sequence is equal to next in line
        if (sequence_number == current->get_tcp_sequence_number()) {
            cout << "c" << endl;
            int count = current->get_data_length_bytes();
            cout << "d" << endl;
            data.append((const char*) current->get_data(), count);
            cout << "e" << endl;
            sequence_number += count;
            cout << "f" << endl;

        } else if (less_than(current->get_tcp_sequence_number(), sequence_number)) {
            cout << "g" << endl;
            int count = current->get_data_length_bytes();
            cout << "h" << endl;
            int difference = sequence_number - current->get_tcp_sequence_number();
            cout << "i" << endl;
            const char* d = (const char*) current->get_data() + difference;
            cout << "j" << endl;
            int added = count - difference;
            cout << "k" << endl;
            data.append(d, added);
            cout << "l" << endl;
            sequence_number += added;
            cout << "m" << endl;

        } else {
            // gap, we are finished
            cout << "n" << endl;
        }
        ++itr;
    }
    buffer_.erase(buffer_.begin(), itr);
    cout << "o" << endl;


    return data;
}
