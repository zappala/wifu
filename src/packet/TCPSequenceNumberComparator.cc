#include "packet/TCPSequenceNumberComparator.h"

bool TCPSequenceNumberComparator::operator()(TCPPacket* const& p1, TCPPacket* const& p2) {
     return less_than(p1->get_tcp_sequence_number(), p2->get_tcp_sequence_number());
}
