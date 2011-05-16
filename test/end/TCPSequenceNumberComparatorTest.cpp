#ifndef _TCPSEQUENCENUMBERCOMPARATORTEST
#define	_TCPSEQUENCENUMBERCOMPARATORTEST

#include "gtest/gtest.h"
#include "packet/TCPSequenceNumberComparator.h"
#include "PriorityQueue.h"
#include "../headers/RandomStringGenerator.h"

using namespace std;

TCPPacket* get_packet(u_int32_t sequence_number, string& data) {
    TCPPacket* p = new TCPPacket();
    p->set_tcp_sequence_number(sequence_number);
    p->set_data((unsigned char*) data.data(), data.size());
    return p;
}

namespace {

    TEST(TCPSequenceNumberComparator, compareSequenceNumbers) {
        PriorityQueue<TCPPacket*, TCPSequenceNumberComparator> q;

        string data = "This is the data";
        for (int i = 0; i < 1000; i++) {
            q.enqueue(get_packet(rand(), data));
        }

        u_int32_t current = 0;

        while (!q.is_empty()) {
            TCPPacket* p = q.dequeue();
            EXPECT_LE(current, p->get_tcp_sequence_number());
            current = p->get_tcp_sequence_number();
        }
    }

    TEST(TCPSequenceNumberComparator, compareDataLength) {
        PriorityQueue<TCPPacket*, TCPSequenceNumberComparator> q;


        for (int i = 0; i < 1000; i++) {
            string data = RandomStringGenerator::get_data(rand() % 100);
            q.enqueue(get_packet(100, data));
        }

        u_int32_t current = 0;

        while (!q.is_empty()) {
            TCPPacket* p = q.dequeue();
            EXPECT_LE(current, p->get_data_length_bytes());
            current = p->get_data_length_bytes();
        }
    }

    TEST(TCPSequenceNumberComparator, compareSequenceNumberAndDataLength) {
        PriorityQueue<TCPPacket*, TCPSequenceNumberComparator> q;

        for (int i = 0; i < 100; i++) {
            string data = RandomStringGenerator::get_data(rand() % 10);
            u_int32_t seq = rand() % 10;
            q.enqueue(get_packet(seq, data));
        }

        u_int32_t current_seq = 0;
        int current_length = 0;

        while (!q.is_empty()) {
            TCPPacket* p = q.dequeue();

            EXPECT_LE(current_length, p->get_data_length_bytes());
            current_length = current_seq < p->get_tcp_sequence_number() ? p->get_data_length_bytes() : 0;

            EXPECT_LE(current_seq, p->get_tcp_sequence_number());
            current_seq = p->get_tcp_sequence_number();
        }
    }
}


#endif	/* _TCPSEQUENCENUMBERCOMPARATORTEST */


