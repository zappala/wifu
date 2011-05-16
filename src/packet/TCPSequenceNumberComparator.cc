#include "packet/TCPSequenceNumberComparator.h"

bool TCPSequenceNumberComparator::operator()(TCPPacket*& p1, TCPPacket*& p2) {
    if(less_than(p2->get_tcp_sequence_number(), p1->get_tcp_sequence_number())) {
        return true;
    }

    if(p2->get_tcp_sequence_number() == p1->get_tcp_sequence_number() &&
            p2->get_data_length_bytes() < p1->get_data_length_bytes()) {
        return true;
    }
    
    return false;
}
