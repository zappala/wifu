#include "packet/TCPSequenceNumberComparator.h"

bool TCPSequenceNumberComparator::operator()(TCPPacket*& p1, TCPPacket*& p2) {
    return less_than(p2->get_tcp_sequence_number(), p1->get_tcp_sequence_number());
}
