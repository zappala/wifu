/* 
 * File:   IPPacketTest.h
 * Author: rbuck
 *
 * Created on January 28, 2011, 8:30 AM
 */

#ifndef _IPPACKETTEST_H
#define	_IPPACKETTEST_H

#include "UnitTest++.h"
#include "../headers/packet/IPPacket.h"
#include "../headers/AddressPort.h"

using namespace std;

namespace {

    SUITE(IPPacket) {

        class IPPacketHelper {
        public:

            IPPacketHelper() : data("This is some cool data") {
                struct iphdr* header = (struct iphdr*) buffer;
                header->check = 1;
                header->daddr = htonl(2);
                header->frag_off = htons(3);
                header->id = htons(4);
                header->ihl = 5;
                header->protocol = 6;
                header->saddr = htonl(7);
                header->tos = 8;

                header->ttl = 10;
                header->version = 11;
                unsigned char* payload = buffer + sizeof (struct iphdr);
                memcpy(payload, data, strlen(data));

                length_ = strlen(data) + sizeof (struct iphdr);
                header->tot_len = htons(length_);
            }

            unsigned char* get_buffer() {
                return buffer;
            }

            unsigned short length() {
                return length_;
            }

            const char* get_data() {
                return data;
            }

        private:
            unsigned short length_;
            unsigned char buffer[1500];
            const char* data;
        };

        TEST(EmptyConstructor) {
            IPPacket p;
            CHECK_EQUAL(4, p.get_ip_version());
            CHECK_EQUAL(sizeof (struct iphdr) / 4, p.get_ip_length());
            CHECK_EQUAL(sizeof (struct iphdr), p.get_ip_length_bytes());
            CHECK_EQUAL(0, p.get_ip_tos());
            CHECK_EQUAL(0, p.get_ip_fragmentation_offset());
            CHECK_EQUAL(MAX_TTL, p.get_ip_ttl());
        }

        void test_constructor(IPPacket & p, IPPacketHelper & helper) {
            CHECK_EQUAL(1, p.get_ip_checksum());
            CHECK_EQUAL(2, p.get_ip_destination_address());
            CHECK_EQUAL(3, p.get_ip_fragmentation_offset());
            CHECK_EQUAL(4, p.get_ip_identifier());
            CHECK_EQUAL(5, p.get_ip_length());
            CHECK_EQUAL(5 * 4, p.get_ip_length_bytes());
            CHECK_EQUAL(6, p.get_ip_protocol());
            CHECK_EQUAL(7, p.get_ip_source_address());
            CHECK_EQUAL(8, p.get_ip_tos());
            CHECK_EQUAL(helper.length(), p.get_ip_datagram_length());
            CHECK_EQUAL(10, p.get_ip_ttl());
            CHECK_EQUAL(11, p.get_ip_version());
            CHECK(!strncmp(helper.get_data(), (const char*) p.get_next_header(), strlen(helper.get_data())));
        }

        TEST(BufferConstructor) {
            IPPacketHelper helper;
            IPPacket p(helper.get_buffer(), helper.length());
            test_constructor(p, helper);
        }

        TEST(CopyConstructor) {
            IPPacketHelper helper;
            IPPacket p(helper.get_buffer(), helper.length());
            IPPacket c(p);
            test_constructor(c, helper);
        }

        TEST(GetPayload) {
            IPPacketHelper helper;
            IPPacket p(helper.get_buffer(), helper.length());
            CHECK(!memcmp(helper.get_buffer(), p.get_payload(), helper.length()));
        }

        TEST(GetNextHeader) {
            IPPacketHelper helper;
            IPPacket p(helper.get_buffer(), helper.length());
            const char* data = helper.get_data();
            CHECK(!strncmp(data, (const char*) p.get_next_header(), strlen(data)));
        }

        TEST(IPVersion) {
            IPPacket p;
            int a = 5;
            p.set_ip_version(a);
            CHECK_EQUAL(a, p.get_ip_version());
        }

        TEST(IPLength) {
            IPPacket p;
            int a = 5;
            p.set_ip_length(a);
            CHECK_EQUAL(a, p.get_ip_length());
            CHECK_EQUAL(a * 4, p.get_ip_length_bytes());
        }

        TEST(TOS) {
            IPPacket p;
            int a = 5;
            p.set_ip_tos(a);
            CHECK_EQUAL(a, p.get_ip_tos());
        }

        TEST(DatagramLength) {
            IPPacket p;
            int a = 500;
            p.set_ip_datagram_length(a);
            CHECK_EQUAL(a, p.get_ip_datagram_length());
        }

        TEST(ID) {
            IPPacket p;
            int a = 50;
            p.set_ip_identifier(a);
            CHECK_EQUAL(a, p.get_ip_identifier());
        }

        TEST(FragmentationOffset) {
            IPPacket p;
            int a = 50;
            p.set_ip_fragmentation_offset(a);
            CHECK_EQUAL(a, p.get_ip_fragmentation_offset());
        }

        TEST(TTL) {
            IPPacket p;
            int a = MAX_TTL;
            p.set_ip_ttl(a);
            CHECK_EQUAL(a, p.get_ip_ttl());
        }

        TEST(Protocol) {
            IPPacket p;
            int a = 6;
            p.set_ip_protocol(a);
            CHECK_EQUAL(a, p.get_ip_protocol());
        }

        TEST(Checksum) {
            IPPacket p;
            int a = 800;
            p.set_ip_checksum(a);
            CHECK_EQUAL(a, p.get_ip_checksum());
        }

        TEST(SourceAddress) {
            // string to string
            AddressPort a("192.168.0.1", 5000);
            IPPacket p;
            p.set_ip_source_address_s(a.get_address());
            CHECK_EQUAL(a.get_address(), p.get_ip_source_address_s());

            // uint to uint
            u_int32_t address = ntohl(a.get_network_struct().sin_addr.s_addr);
            p.set_ip_source_address(address);
            CHECK_EQUAL(address, p.get_ip_source_address());

            // uint to string
            p.set_ip_source_address(address);
            CHECK_EQUAL(a.get_address(), p.get_ip_source_address_s());

            // string to uint
            p.set_ip_source_address_s(a.get_address());
            CHECK_EQUAL(address, p.get_ip_source_address());
        }

        TEST(DestinationAddress) {
            AddressPort a("192.168.0.2", 5000);
            IPPacket p;
            p.set_ip_destination_address_s(a.get_address());
            CHECK_EQUAL(a.get_address(), p.get_ip_destination_address_s());

            // uint to uint
            u_int32_t address = ntohl(a.get_network_struct().sin_addr.s_addr);
            p.set_ip_destination_address(address);
            CHECK_EQUAL(address, p.get_ip_destination_address());

            // uint to string
            p.set_ip_destination_address(address);
            CHECK_EQUAL(a.get_address(), p.get_ip_destination_address_s());

            // string to uint
            p.set_ip_destination_address_s(a.get_address());
            CHECK_EQUAL(address, p.get_ip_destination_address());
        }
    }
}

#endif	/* _IPPACKETTEST_H */

