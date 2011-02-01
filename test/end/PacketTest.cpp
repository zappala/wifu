/* 
 * File:   PacketFixture.h
 * Author: rbuck
 *
 * Created on November 19, 2010, 12:13 PM
 */

#ifndef _PACKETTEST_H
#define	_PACKETTEST_H

#include "../headers/Packet.h"
#include "../headers/AddressPort.h"

#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#define DATA_LENGTH 16

using namespace std;

namespace {
	class PacketFixture : public ::testing::Test {
	protected:
		virtual void SetUp() {
			data_length = DATA_LENGTH;

			message = "this is the data";
			memcpy(data, message, data_length);

			source_ = new AddressPort("127.0.0.1", 1);
			dest_ = new AddressPort("127.0.0.2", 2);

			packet = new Packet(source_, dest_, data, data_length);
		}

		virtual void TearDown() {
			delete source_;
			delete dest_;
			delete packet;
		}

		Packet* packet;
		unsigned char data[DATA_LENGTH];
		const char* message;
		int data_length;
		AddressPort* source_;
		AddressPort* dest_;
	};


	TEST_F(PacketFixture, PacketToBytes) {
		unsigned char* bytes = packet->to_bytes();
		struct wifu_end_header header;
		header.virtual_dest_port = dest_->get_port();
		header.virtual_src_port = source_->get_port();

		// Check header
		struct wifu_end_header* to_check = (struct wifu_end_header*) bytes;
		ASSERT_EQ(header.virtual_dest_port, to_check->virtual_dest_port);
		ASSERT_EQ(header.virtual_src_port, to_check->virtual_src_port);

		// Check data
		const char* message_to_check = (const char*)(bytes + sizeof(struct wifu_end_header));
		ASSERT_EQ(message, message_to_check);
	}


	TEST_F(PacketFixture, PacketLength) {
		ASSERT_EQ(data_length + sizeof(struct wifu_end_header), packet->packet_length());
	}

	TEST_F(PacketFixture, PacketDataLength) {
		ASSERT_EQ(data_length, packet->data_length());
	}

	TEST_F(PacketFixture, PacketGetSource) {
		ASSERT_TRUE(*(source_) == *(packet->get_source()));
	}

	TEST_F(PacketFixture, PacketGetDest) {
		ASSERT_TRUE(*(dest_) == *(packet->get_destination()));
	}

	TEST_F(PacketFixture, PacketGetData) {
		ASSERT_EQ(message, (const char*)packet->get_data());
	}
}

#endif	/* _PACKETTEST_H */

