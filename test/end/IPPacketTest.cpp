/*
 * File:   IPPacketTest.h
 * Author: rbuck
 *
 * Created on January 28, 2011, 8:30 AM
 */

#ifndef _IPPACKETTEST_H
#define	_IPPACKETTEST_H

#include "gtest/gtest.h"
#include "packet/IPPacket.h"
#include "AddressPort.h"
#include <iostream>

using namespace std;

namespace {

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
        unsigned char buffer[MTU];
        const char* data;
    };

    TEST(IPPacketTest, EmptyConstructor) {
        IPPacket p;
        ASSERT_EQ(4, p.get_ip_version());
        ASSERT_EQ(sizeof (struct iphdr) / 4, p.get_ip_header_length_words());
        ASSERT_EQ(sizeof (struct iphdr), p.get_ip_header_length_bytes());
        ASSERT_EQ(0, p.get_ip_tos());
        ASSERT_EQ(0, p.get_ip_fragmentation_offset());
        ASSERT_EQ(MAX_TTL, p.get_ip_ttl());
    }

    void test_constructor(IPPacket & p, IPPacketHelper & helper) {
        ASSERT_EQ(1, p.get_ip_checksum());
        ASSERT_EQ(2, p.get_ip_destination_address());
        ASSERT_EQ(3, p.get_ip_fragmentation_offset());
        ASSERT_EQ(4, p.get_ip_identifier());
        ASSERT_EQ(5, p.get_ip_header_length_words());
        ASSERT_EQ(5 * 4, p.get_ip_header_length_bytes());
        ASSERT_EQ(6, p.get_ip_protocol());
        ASSERT_EQ(7, p.get_ip_source_address());
        ASSERT_EQ(8, p.get_ip_tos());
        ASSERT_EQ(helper.length(), p.get_ip_tot_length());
        ASSERT_EQ(10, p.get_ip_ttl());
        ASSERT_EQ(11, p.get_ip_version());
        ASSERT_TRUE(!strncmp(helper.get_data(), (const char*) p.get_data(), strlen(helper.get_data())));
    }

    TEST(IPPacketTest, BufferConstructor) {
        IPPacketHelper helper;
        IPPacket p(helper.get_buffer(), helper.length());
        test_constructor(p, helper);
    }

    TEST(IPPacketTest, CopyConstructor) {
        IPPacketHelper helper;
        IPPacket p(helper.get_buffer(), helper.length());
        IPPacket c(p);
        test_constructor(c, helper);
    }

    TEST(IPPacketTest, GetPayload) {
        IPPacketHelper helper;
        IPPacket p(helper.get_buffer(), helper.length());
        ASSERT_TRUE(!memcmp(helper.get_buffer(), p.get_payload(), helper.length()));
    }

    TEST(IPPacketTest, GetData) {
        IPPacketHelper helper;
        IPPacket p(helper.get_buffer(), helper.length());
        ASSERT_TRUE(!strncmp(helper.get_data(), (const char*) p.get_data(), strlen(helper.get_data())));
    }

    TEST(IPPacketTest, GetNextHeader) {
        IPPacketHelper helper;
        IPPacket p(helper.get_buffer(), helper.length());
        const char* data = helper.get_data();
        ASSERT_TRUE(!strncmp(data, (const char*) p.get_next_header(), strlen(data)));
    }

    TEST(IPPacketTest, IPVersion) {
        IPPacket p;
        int a = 5;
        p.set_ip_version(a);
        ASSERT_EQ(a, p.get_ip_version());
    }

    TEST(IPPacketTest, IPLength) {
        IPPacket p;
        int a = 5;
        p.set_ip_header_length_words(a);
        ASSERT_EQ(a, p.get_ip_header_length_words());
        ASSERT_EQ(a * 4, p.get_ip_header_length_bytes());
    }

    TEST(IPPacketTest, TOS) {
        IPPacket p;
        int a = 5;
        p.set_ip_tos(a);
        ASSERT_EQ(a, p.get_ip_tos());
    }

    TEST(IPPacketTest, DatagramLength) {
        IPPacket p;
        int a = 500;
        p.set_ip_tot_length(a);
        ASSERT_EQ(a, p.get_ip_tot_length());
    }

    TEST(IPPacketTest, ID) {
        IPPacket p;
        int a = 50;
        p.set_ip_identifier(a);
        ASSERT_EQ(a, p.get_ip_identifier());
    }

    TEST(IPPacketTest, FragmentationOffset) {
        IPPacket p;
        int a = 50;
        p.set_ip_fragmentation_offset(a);
        ASSERT_EQ(a, p.get_ip_fragmentation_offset());
    }

    TEST(IPPacketTest, TTL) {
        IPPacket p;
        int a = MAX_TTL;
        p.set_ip_ttl(a);
        ASSERT_EQ(a, p.get_ip_ttl());
    }

    TEST(IPPacketTest, Protocol) {
        IPPacket p;
        int a = 6;
        p.set_ip_protocol(a);
        ASSERT_EQ(a, p.get_ip_protocol());
    }

    TEST(IPPacketTest, Checksum) {
        IPPacket p;
        int a = 800;
        p.set_ip_checksum(a);
        ASSERT_EQ(a, p.get_ip_checksum());
    }

    TEST(IPPacketTest, SourceAddress) {
        // string to string
        AddressPort a("192.168.0.1", 5000);
        IPPacket p;
        p.set_ip_source_address_s(a.get_address());
        ASSERT_EQ(a.get_address(), p.get_ip_source_address_s());

        // uint to uint
        u_int32_t address = ntohl(a.get_network_struct().sin_addr.s_addr);
        p.set_ip_source_address(address);
        ASSERT_EQ(address, p.get_ip_source_address());

        // uint to string
        p.set_ip_source_address(address);
        ASSERT_EQ(a.get_address(), p.get_ip_source_address_s());

        // string to uint
        p.set_ip_source_address_s(a.get_address());
        ASSERT_EQ(address, p.get_ip_source_address());
    }

    TEST(IPPacketTest, DestinationAddress) {
        AddressPort a("192.168.0.2", 5000);
        IPPacket p;
        p.set_ip_destination_address_s(a.get_address());
        ASSERT_EQ(a.get_address(), p.get_ip_destination_address_s());

        // uint to uint
        u_int32_t address = ntohl(a.get_network_struct().sin_addr.s_addr);
        p.set_ip_destination_address(address);
        ASSERT_EQ(address, p.get_ip_destination_address());

        // uint to string
        p.set_ip_destination_address(address);
        ASSERT_EQ(a.get_address(), p.get_ip_destination_address_s());

        // string to uint
        p.set_ip_destination_address_s(a.get_address());
        ASSERT_EQ(address, p.get_ip_destination_address());
    }

    TEST(IPPacketTest, MaxDataSizeTest) {
        IPPacket p;
        // MTU - IP header size bytes
        int exp = 1500 - 20;

        ASSERT_EQ(exp, p.max_data_length());
    }

    TEST(IPPacketTest, equals) {
        IPPacketHelper helper;

        IPPacket packet1;
        packet1.set_data(helper.get_buffer(), helper.length());

        IPPacket packet2;
        packet2.set_data(helper.get_buffer(), helper.length());

//        packet2.set_ip_protocol(16);

        ASSERT_EQ(packet1, packet2);

        cout << packet2.to_s_format();
    }
}

#endif	/* _IPPACKETTEST_H */


