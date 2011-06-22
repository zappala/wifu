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

    class IPPacketTest : public ::testing::Test {
    protected:

        IPPacketTest() : data("This is some cool data") {
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

    TEST(IPPacket, EmptyConstructor) {
        IPPacket p;
        ASSERT_EQ(4, p.get_ip_version());
        ASSERT_EQ(sizeof (struct iphdr) / 4, p.get_ip_header_length_words());
        ASSERT_EQ(sizeof (struct iphdr), p.get_ip_header_length_bytes());
        ASSERT_EQ(0, p.get_ip_tos());
        ASSERT_EQ(0, p.get_ip_fragmentation_offset());
        ASSERT_EQ(MAX_TTL, p.get_ip_ttl());
    }

    //    void test_constructor(IPPacket & p, IPPacketTest & helper) {
    //        ASSERT_EQ(1, p.get_ip_checksum());
    //        ASSERT_EQ(2, p.get_ip_destination_address());
    //        ASSERT_EQ(3, p.get_ip_fragmentation_offset());
    //        ASSERT_EQ(4, p.get_ip_identifier());
    //        ASSERT_EQ(5, p.get_ip_header_length_words());
    //        ASSERT_EQ(5 * 4, p.get_ip_header_length_bytes());
    //        ASSERT_EQ(6, p.get_ip_protocol());
    //        ASSERT_EQ(7, p.get_ip_source_address());
    //        ASSERT_EQ(8, p.get_ip_tos());
    //        ASSERT_EQ(helper.length(), p.get_ip_tot_length());
    //        ASSERT_EQ(10, p.get_ip_ttl());
    //        ASSERT_EQ(11, p.get_ip_version());
    //        ASSERT_TRUE(!strncmp(helper.get_data(), (const char*) p.get_data(), strlen(helper.get_data())));
    //    }
    //
    //    TEST(IPPacketTest, BufferConstructor) {
    //        IPPacketTest helper;
    //        IPPacket p(helper.get_buffer(), helper.length());
    //        test_constructor(p, helper);
    //    }
    //
    //    TEST(IPPacketTest, CopyConstructor) {
    //        IPPacketTest helper;
    //        IPPacket p(helper.get_buffer(), helper.length());
    //        IPPacket c(p);
    //        test_constructor(c, helper);
    //    }

    TEST_F(IPPacketTest, GetPayload) {
        IPPacket p(get_buffer(), length());
        ASSERT_TRUE(!memcmp(get_buffer(), p.get_payload(), length()));
    }

    TEST_F(IPPacketTest, GetData) {
        IPPacket p(get_buffer(), length());
        ASSERT_TRUE(!strncmp(get_data(), (const char*) p.get_data(), strlen(get_data())));
    }

    TEST_F(IPPacketTest, GetNextHeader) {
        IPPacket p(get_buffer(), length());
        ASSERT_TRUE(!strncmp(get_data(), (const char*) p.get_next_header(), strlen(get_data())));
    }

    TEST(IPPacket, IPVersion) {
        IPPacket p;
        int a = 5;
        p.set_ip_version(a);
        ASSERT_EQ(a, p.get_ip_version());
    }

    TEST(IPPacket, IPLength) {
        IPPacket p;
        int a = 5;
        p.set_ip_header_length_words(a);
        ASSERT_EQ(a, p.get_ip_header_length_words());
        ASSERT_EQ(a * 4, p.get_ip_header_length_bytes());
    }

    TEST(IPPacket, TOS) {
        IPPacket p;
        int a = 5;
        p.set_ip_tos(a);
        ASSERT_EQ(a, p.get_ip_tos());
    }

    TEST(IPPacket, DatagramLength) {
        IPPacket p;
        int a = 500;
        p.set_ip_tot_length(a);
        ASSERT_EQ(a, p.get_ip_tot_length());
    }

    TEST(IPPacket, ID) {
        IPPacket p;
        int a = 50;
        p.set_ip_identifier(a);
        ASSERT_EQ(a, p.get_ip_identifier());
    }

    TEST(IPPacket, FragmentationOffset) {
        IPPacket p;
        int a = 50;
        p.set_ip_fragmentation_offset(a);
        ASSERT_EQ(a, p.get_ip_fragmentation_offset());
    }

    TEST(IPPacket, TTL) {
        IPPacket p;
        int a = MAX_TTL;
        p.set_ip_ttl(a);
        ASSERT_EQ(a, p.get_ip_ttl());
    }

    TEST(IPPacket, Protocol) {
        IPPacket p;
        int a = 6;
        p.set_ip_protocol(a);
        ASSERT_EQ(a, p.get_ip_protocol());
    }

    TEST(IPPacket, Checksum) {
        IPPacket p;
        int a = 800;
        p.set_ip_checksum(a);
        ASSERT_EQ(a, p.get_ip_checksum());
    }

    TEST(IPPacket, SourceAddress) {
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

    TEST(IPPacket, DestinationAddress) {
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

    TEST(IPPacket, MaxDataSizeTest) {
        IPPacket p;
        // MTU - IP header size bytes
        int exp = 1500 - 20;

        ASSERT_EQ(exp, p.max_data_length());
    }

    TEST(IPPacket, checksumCalculation) {
        IPPacket p1, p2;
        p1.calculate_and_set_checksum();
        p2.calculate_and_set_checksum();

        ASSERT_EQ(p1.get_ip_checksum(), p2.get_ip_checksum());
        ASSERT_TRUE(p1.is_valid_checksum());
        ASSERT_TRUE(p2.is_valid_checksum());

        // change version
        u_int8_t version = p1.get_ip_version();
        p1.set_ip_version(12);
        ASSERT_FALSE(p1.is_valid_checksum());
        p1.set_ip_version(version);
        ASSERT_TRUE(p1.is_valid_checksum());

        // change header length
        u_int8_t length = p1.get_ip_header_length_words();
        p1.set_ip_header_length_words(12);
        ASSERT_FALSE(p1.is_valid_checksum());
        p1.set_ip_header_length_words(length);
        ASSERT_TRUE(p1.is_valid_checksum());

        // change tos
        u_int8_t tos = p1.get_ip_tos();
        p1.set_ip_tos(12);
        ASSERT_FALSE(p1.is_valid_checksum());
        p1.set_ip_tos(tos);
        ASSERT_TRUE(p1.is_valid_checksum());

        // change datagram length (bytes)
        u_int16_t dlength = p1.get_ip_tot_length();
        p1.set_ip_tot_length(USHRT_MAX);
        ASSERT_FALSE(p1.is_valid_checksum());
        p1.set_ip_tot_length(dlength);
        ASSERT_TRUE(p1.is_valid_checksum());

        // change identifier
        u_int16_t id = p1.get_ip_identifier();
        p1.set_ip_identifier(3);
        ASSERT_FALSE(p1.is_valid_checksum());
        p1.set_ip_identifier(id);
        ASSERT_TRUE(p1.is_valid_checksum());

        // change fragmentation offset
        u_int16_t frag = p1.get_ip_fragmentation_offset();
        p1.set_ip_fragmentation_offset(3);
        ASSERT_FALSE(p1.is_valid_checksum());
        p1.set_ip_fragmentation_offset(frag);
        ASSERT_TRUE(p1.is_valid_checksum());

        // change TTL
        u_int8_t ttl = p1.get_ip_ttl();
        p1.set_ip_ttl(3);
        ASSERT_FALSE(p1.is_valid_checksum());
        p1.set_ip_ttl(ttl);
        ASSERT_TRUE(p1.is_valid_checksum());

        // change protocol
        u_int8_t protocol = p1.get_ip_protocol();
        p1.set_ip_protocol(3);
        ASSERT_FALSE(p1.is_valid_checksum());
        p1.set_ip_protocol(protocol);
        ASSERT_TRUE(p1.is_valid_checksum());

        // change checksum
        u_int16_t check = p1.get_ip_checksum();
        p1.set_ip_checksum(3);
        ASSERT_FALSE(p1.is_valid_checksum());
        p1.set_ip_checksum(check);
        ASSERT_TRUE(p1.is_valid_checksum());

        // change source address
        string source_address = p1.get_ip_source_address_s();
        p1.set_ip_source_address_s("192.168.0.1");
        ASSERT_FALSE(p1.is_valid_checksum());
        p1.set_ip_source_address_s(source_address);
        ASSERT_TRUE(p1.is_valid_checksum());

        // change destination address
        string destination_address = p1.get_ip_destination_address_s();
        p1.set_ip_destination_address_s("192.168.0.1");
        ASSERT_FALSE(p1.is_valid_checksum());
        p1.set_ip_destination_address_s(destination_address);
        ASSERT_TRUE(p1.is_valid_checksum());


    }
}

#endif	/* _IPPACKETTEST_H */


