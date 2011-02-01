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
	TCPPacket * get_packet() {
		AddressPort* source = new AddressPort("127.0.0.1", 5000);
		AddressPort* dest = new AddressPort("127.0.0.1", 5001);
		string data = "Hello";
		return new TCPPacket(source, dest, (unsigned char*) data.c_str(), data.size());
	}

	TEST(TCPPacketTest, SequenceNumberTest) {
		RandomNumberSet<u_int32_t> random;
		TCPPacket* p = get_packet();

		u_int32_t value = random.get();
		p->set_tcp_sequence_number(value);
		ASSERT_EQ(value, p->get_tcp_sequence_number());
	}

	TEST(TCPPacketTest, AckNumberTest) {
		RandomNumberSet<u_int32_t> random;
		TCPPacket* p = get_packet();

		u_int32_t value = random.get();
		p->set_tcp_ack_number(value);
		ASSERT_EQ(value, p->get_tcp_ack_number());
	}

	TEST(TCPPacketTest, TCPHeaderLengthBytesTest) {
		TCPPacket* p = get_packet();
		ASSERT_EQ(TCP_HEADER_SIZE, p->get_tcp_header_length_bytes());
	}

	TEST(TCPPacketTest, TCPHeaderLengthTest) {
		TCPPacket* p = get_packet();
		ASSERT_EQ(TCP_HEADER_SIZE / 4, p->get_tcp_header_length());
	}

	TEST(TCPPacketTest, URG) {
		TCPPacket* p = get_packet();

		p->set_tcp_urg(true);
		ASSERT_TRUE(p->is_tcp_urg());

		p->set_tcp_urg(false);
		ASSERT_TRUE(!p->is_tcp_urg());
	}

	TEST(TCPPacketTest, ACK) {
		TCPPacket* p = get_packet();

		p->set_tcp_ack(true);
		ASSERT_TRUE(p->is_tcp_ack());

		p->set_tcp_ack(false);
		ASSERT_TRUE(!p->is_tcp_ack());
	}

	TEST(TCPPacketTest, PSH) {
		TCPPacket* p = get_packet();

		p->set_tcp_psh(true);
		ASSERT_TRUE(p->is_tcp_psh());

		p->set_tcp_psh(false);
		ASSERT_TRUE(!p->is_tcp_psh());
	}

	TEST(TCPPacketTest, RST) {
		TCPPacket* p = get_packet();

		p->set_tcp_rst(true);
		ASSERT_TRUE(p->is_tcp_rst());

		p->set_tcp_rst(false);
		ASSERT_TRUE(!p->is_tcp_rst());
	}

	TEST(TCPPacketTest, SYN) {
		TCPPacket* p = get_packet();

		p->set_tcp_syn(true);
		ASSERT_TRUE(p->is_tcp_syn());

		p->set_tcp_syn(false);
		ASSERT_TRUE(!p->is_tcp_syn());
	}

	TEST(TCPPacketTest, FIN) {
		TCPPacket* p = get_packet();

		p->set_tcp_fin(true);
		ASSERT_TRUE(p->is_tcp_fin());

		p->set_tcp_fin(false);
		ASSERT_TRUE(!p->is_tcp_fin());
	}

	TEST(TCPPacketTest, ReceiveWindowTest) {
		RandomNumberSet<u_int16_t> random;
		TCPPacket* p = get_packet();

		u_int16_t value = random.get();
		p->set_tcp_receive_window_size(value);
		ASSERT_EQ(value, p->get_tcp_receive_window_size());
	}

	TEST(TCPPacketTest, ChecksumTest) {
		RandomNumberSet<u_int16_t> random;
		TCPPacket* p = get_packet();

		u_int16_t value = random.get();
		p->set_tcp_checksum(value);
		ASSERT_EQ(value, p->get_tcp_checksum());
	}

	TEST(TCPPacketTest, UrgentPointerTest) {
		RandomNumberSet<u_int16_t> random;
		TCPPacket* p = get_packet();

		u_int16_t value = random.get();
		p->set_tcp_urgent_pointer(value);
		ASSERT_EQ(value, p->get_tcp_urgent_pointer());
	}
}

#endif	/* _TCPPACKETTEST_H */


