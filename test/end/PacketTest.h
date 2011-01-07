/* 
 * File:   PacketTest.h
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

#include "UnitTest++.h"

#define DATA_LENGTH 16

using namespace std;

namespace {

    SUITE(Packet) {

        class PacketFixture {
        public:

            PacketFixture() {
                data_length = DATA_LENGTH;

                message = "this is the data";
                memcpy(data, message, data_length);

                source_ = new AddressPort("127.0.0.1", 1);
                dest_ = new AddressPort("127.0.0.2", 2);

                packet = new Packet(source_, dest_, data, data_length);
            }

            Packet* packet;
            unsigned char data[DATA_LENGTH];
            const char* message;
            int data_length;
            AddressPort* source_;
            AddressPort* dest_;
        };


        TEST(PacketToBytesTest) {
            PacketFixture f;

            unsigned char* bytes = f.packet->to_bytes();
            struct wifu_end_header header;
            header.virtual_dest_port = f.dest_->get_port();
            header.virtual_src_port = f.source_->get_port();

            // Check header
            struct wifu_end_header* to_check = (struct wifu_end_header*) bytes;
            CHECK_EQUAL(header.virtual_dest_port, to_check->virtual_dest_port);
            CHECK_EQUAL(header.virtual_src_port, to_check->virtual_src_port);

            // Check data
            const char* message_to_check = (const char*)(bytes + sizeof(struct wifu_end_header));
            CHECK_EQUAL(f.message, message_to_check);
        }


        TEST(PacketLengthTest) {
            PacketFixture f;
            CHECK_EQUAL(f.data_length, f.packet->length());
        }

        TEST(PacketGetSourceTest) {
            PacketFixture f;

            CHECK(*(f.source_) == *(f.packet->get_source()));
        }

        TEST(PacketGetDestTest) {
            PacketFixture f;

            CHECK(*(f.dest_) == *(f.packet->get_destination()));
        }

        TEST(PacketGetDataTest) {
            PacketFixture f;
            CHECK_EQUAL(f.message, (const char*)f.packet->get_data());
        }
    }
}

#endif	/* _PACKETTEST_H */

