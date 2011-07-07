#ifndef _TCPSEQUENCENUMBERCOMPARATORTEST
#define	_TCPSEQUENCENUMBERCOMPARATORTEST

#include "gtest/gtest.h"
#include "packet/TCPSequenceNumberComparator.h"
#include "PriorityQueue.h"
#include "../headers/RandomStringGenerator.h"
#include "../headers/HelperFunctions.h"
using namespace std;



namespace {

    TEST(TCPSequenceNumberComparator, compareSequenceNumbers) {
        PriorityQueue<TCPPacket*, TCPSequenceNumberComparator> q;

        gcstring data = "This is the data";
        for (int i = 0; i < 1000; i++) {
            q.enqueue(HelperFunctions::get_tcp_packet_with_data(rand(), data));
        }

        u_int32_t current = 0;

        while (!q.is_empty()) {
            TCPPacket* p = q.dequeue();
            EXPECT_LE(current, p->get_tcp_sequence_number());
            current = p->get_tcp_sequence_number();
        }
    }
}


#endif	/* _TCPSEQUENCENUMBERCOMPARATORTEST */


