/* 
 * File:   TCPPacketTest.h
 * Author: rbuck
 *
 * Created on January 7, 2011, 3:03 PM
 */

#ifndef _TCPPACKETTEST_H
#define	_TCPPACKETTEST_H

#include <iostream>

#include "gtest/gtest.h"
#include "../headers/TCPPacket.h"
#include "../headers/RandomNumberSet.h"

using namespace std;

namespace {

    SUITE(TCPPacket) {

        TCPPacket * get_packet() {
            AddressPort* source = new AddressPort("127.0.0.1", 5000);
            AddressPort* dest = new AddressPort("127.0.0.1", 5001);
            string data = "Hello";
            return new TCPPacket(source, dest, (unsigned char*) data.c_str(), data.size());
        }

        TEST(SequenceNumberTest) {
            RandomNumberSet<u_int32_t> random;
            TCPPacket* p = get_packet();

            u_int32_t value = random.get();
            p->set_tcp_sequence_number(value);
            CHECK_EQUAL(value, p->get_tcp_sequence_number());
        }

        TEST(AckNumberTest) {
            RandomNumberSet<u_int32_t> random;
            TCPPacket* p = get_packet();

            u_int32_t value = random.get();
            p->set_tcp_ack_number(value);
            CHECK_EQUAL(value, p->get_tcp_ack_number());
        }

        TEST(TCPHeaderLengthBytesTest) {
            TCPPacket* p = get_packet();
            CHECK_EQUAL(TCP_HEADER_SIZE, p->get_tcp_header_length_bytes());
        }

        TEST(TCPHeaderLengthTest) {
            TCPPacket* p = get_packet();
            CHECK_EQUAL(TCP_HEADER_SIZE / 4, p->get_tcp_header_length());
        }

        TEST(URG) {
            TCPPacket* p = get_packet();

            p->set_tcp_urg(true);
            CHECK(p->is_tcp_urg());

            p->set_tcp_urg(false);
            CHECK(!p->is_tcp_urg());
        }

        TEST(ACK) {
            TCPPacket* p = get_packet();

            p->set_tcp_ack(true);
            CHECK(p->is_tcp_ack());

            p->set_tcp_ack(false);
            CHECK(!p->is_tcp_ack());
        }

        TEST(PSH) {
            TCPPacket* p = get_packet();

            p->set_tcp_psh(true);
            CHECK(p->is_tcp_psh());

            p->set_tcp_psh(false);
            CHECK(!p->is_tcp_psh());
        }

        TEST(RST) {
            TCPPacket* p = get_packet();

            p->set_tcp_rst(true);
            CHECK(p->is_tcp_rst());

            p->set_tcp_rst(false);
            CHECK(!p->is_tcp_rst());
        }

        TEST(SYN) {
            TCPPacket* p = get_packet();

            p->set_tcp_syn(true);
            CHECK(p->is_tcp_syn());

            p->set_tcp_syn(false);
            CHECK(!p->is_tcp_syn());
        }

        TEST(FIN) {
            TCPPacket* p = get_packet();

            p->set_tcp_fin(true);
            CHECK(p->is_tcp_fin());

            p->set_tcp_fin(false);
            CHECK(!p->is_tcp_fin());
        }

        TEST(ReceiveWindowTest) {
            RandomNumberSet<u_int16_t> random;
            TCPPacket* p = get_packet();

            u_int16_t value = random.get();
            p->set_tcp_receive_window_size(value);
            CHECK_EQUAL(value, p->get_tcp_receive_window_size());
        }

        TEST(ChecksumTest) {
            RandomNumberSet<u_int16_t> random;
            TCPPacket* p = get_packet();

            u_int16_t value = random.get();
            p->set_tcp_checksum(value);
            CHECK_EQUAL(value, p->get_tcp_checksum());
        }

        TEST(UrgentPointerTest) {
            RandomNumberSet<u_int16_t> random;
            TCPPacket* p = get_packet();

            u_int16_t value = random.get();
            p->set_tcp_urgent_pointer(value);
            CHECK_EQUAL(value, p->get_tcp_urgent_pointer());
        }
    }
}


#endif	/* _TCPPACKETTEST_H */


