/* 
 * File:   PacketTest.h
 * Author: rbuck
 *
 * Created on November 19, 2010, 12:13 PM
 */

#ifndef _PACKETTEST_H
#define	_PACKETTEST_H

#include "../headers/Packet.h"

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

                header.virtual_src_port = 1;
                header.virtual_dest_port = 2;
                header.packet_length = data_length + END_HEADER_SIZE;

                message = "this is the data";
                memcpy(data, message, data_length);
                source_address = "127.0.0.1";
                dest_address = "127.0.0.2";

                packet = new Packet(&header, data, data_length, source_address, dest_address);


                unsigned char payload[DATA_LENGTH * 10];
                memset(payload, 0, DATA_LENGTH * 10);
                memcpy(payload, &header, END_HEADER_SIZE);
                memcpy(payload + END_HEADER_SIZE, data, data_length);

                packet1 = new Packet(payload, data_length + END_HEADER_SIZE, source_address, dest_address);


            }

            ~PacketFixture() {
                delete packet;
                delete packet1;
            }

            Packet* packet;
            Packet* packet1;
            struct wifu_end_header header;
            unsigned char data[DATA_LENGTH];
            const char* message;
            int data_length;
            string source_address;
            string dest_address;
        };

        void toBytesTest(Packet * p, struct wifu_end_header& header, const char* message, int data_length) {
            unsigned char* bytes = p->to_bytes();

            struct wifu_end_header* test_header = (struct wifu_end_header*) bytes;

            CHECK_EQUAL(header.packet_length, test_header->packet_length);
            CHECK_EQUAL(header.virtual_dest_port, test_header->virtual_dest_port);
            CHECK_EQUAL(header.virtual_src_port, test_header->virtual_src_port);

            bytes += sizeof (struct wifu_end_header);
            CHECK(strncmp((const char*) bytes, message, data_length) == 0);
        }

        TEST_FIXTURE(PacketFixture, PacketToBytesTest) {
            toBytesTest(packet, header, message, data_length);
            toBytesTest(packet1, header, message, data_length);
        }

        void getHeaderTest(Packet * p, struct wifu_end_header& header) {
            struct wifu_end_header* test_header = p->get_header();

            CHECK_EQUAL(header.packet_length, test_header->packet_length);
            CHECK_EQUAL(header.virtual_dest_port, test_header->virtual_dest_port);
            CHECK_EQUAL(header.virtual_src_port, test_header->virtual_src_port);
        }

        TEST_FIXTURE(PacketFixture, PacketGetHeaderTest) {
            getHeaderTest(packet, header);
            getHeaderTest(packet1, header);
        }

        void getLengthTest(Packet* p, int data_length) {
            int expected = data_length + END_HEADER_SIZE;
            CHECK_EQUAL(expected, p->length());
        }

        TEST_FIXTURE(PacketFixture, PacketLengthTest) {
            getLengthTest(packet, data_length);
            getLengthTest(packet1, data_length);
        }

        void getSourceAddressTest(Packet* p, string& source_address) {
            CHECK_EQUAL(source_address, p->get_source_address());
        }

        TEST_FIXTURE(PacketFixture, PacketGetSourceAddressTest) {
            getSourceAddressTest(packet, source_address);
            getSourceAddressTest(packet1, source_address);
        }

        void getDestAddressTest(Packet* p, string& dest_address) {
            CHECK_EQUAL(dest_address, p->get_dest_address());
        }

        TEST_FIXTURE(PacketFixture, PacketGetDestAddressTest) {
            getDestAddressTest(packet, dest_address);
            getDestAddressTest(packet1, dest_address);
        }

        void getSourcePortTest(Packet* p, struct wifu_end_header& header) {
            CHECK_EQUAL(header.virtual_src_port, p->get_source_port());
        }

        TEST_FIXTURE(PacketFixture, PacketGetSourcePortTest) {
            getSourcePortTest(packet, header);
            getSourcePortTest(packet1, header);
        }

        void getDestPortTest(Packet* p, struct wifu_end_header& header) {
            CHECK_EQUAL(header.virtual_dest_port, p->get_dest_port());
        }

        TEST_FIXTURE(PacketFixture, PacketGetDestPortTest) {
            getDestPortTest(packet, header);
            getDestPortTest(packet1, header);
        }

        void getDataTest(Packet* p, const char* message, int data_length) {
            CHECK(strncmp((const char*) message, (const char*) p->get_data(), data_length) == 0);
            CHECK(strcmp((const char*) message, (const char*) p->get_data()) == 0);
        }

        TEST_FIXTURE(PacketFixture, PacketGetDataTest) {
            getDataTest(packet, message, data_length);
            getDataTest(packet1, message, data_length);
        }


    }
}

#endif	/* _PACKETTEST_H */

