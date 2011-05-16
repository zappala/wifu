#ifndef _TCPSEQUENCENUMBERCOMPARATORTEST
#define	_TCPSEQUENCENUMBERCOMPARATORTEST

#include "gtest/gtest.h"
#include "packet/TCPSequenceNumberComparator.h"
#include "PriorityQueue.h"

using namespace std;

TCPPacket* get_packet(u_int32_t sequence_number) {
    TCPPacket* p = new TCPPacket();
    p->set_tcp_sequence_number(sequence_number);
    return p;
}

namespace {
    TEST(TCPSequenceNumberComparator, compare) {
        PriorityQueue<TCPPacket*, TCPSequenceNumberComparator> q;

        for(int i = 0; i < 1000; i++) {
            q.enqueue(get_packet(rand()));
        }

        u_int32_t current = 0;
        
        while(!q.is_empty()) {
            TCPPacket* p = q.dequeue();
            EXPECT_LE(current, p->get_tcp_sequence_number());
            current = p->get_tcp_sequence_number();
        }
    }
}


#endif	/* _TCPSEQUENCENUMBERCOMPARATORTEST */


