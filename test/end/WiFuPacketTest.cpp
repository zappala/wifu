/*
 * File:   WiFuPacketTest.h
 * Author: rbuck
 *
 * Created on January 28, 2011, 8:31 AM
 */

#ifndef _WIFUPACKETTEST_H
#define	_WIFUPACKETTEST_H


#include "gtest/gtest.h"

#include "packet/WiFuPacket.h"
#include "AddressPort.h"

using namespace std;

namespace {

    class WiFuPacketHelper {
    public:

        WiFuPacketHelper() : data("This is some cool data") {
            source_ = new AddressPort("192.168.0.1", 5001);
            dest_ = new AddressPort("192.168.0.2", 5002);

            struct iphdr* header = (struct iphdr*) buffer;
            header->check = 1;
            header->daddr = dest_->get_network_struct().sin_addr.s_addr;
            header->frag_off = htons(3);
            header->id = htons(4);
            header->ihl = 5;
            header->protocol = 6;
            header->saddr = source_->get_network_struct().sin_addr.s_addr;
            header->tos = 8;
            header->ttl = 10;
            header->version = 11;

            struct wifu_packet_header* wheader = (struct wifu_packet_header*) (buffer + sizeof (struct iphdr));
            wheader->dport = htons(dest_->get_port());
            wheader->sport = htons(source_->get_port());

            unsigned char* payload = buffer + (sizeof (struct iphdr) + sizeof (struct wifu_packet_header));
            memcpy(payload, data, strlen(data));

            length_ = strlen(data) + sizeof (struct iphdr) + sizeof (struct wifu_packet_header);
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

        struct wifu_packet_header* get_wifu_header() {
            return wifu_header_;
        }

        AddressPort* get_source() {
            return source_;
        }

        AddressPort* get_dest() {
            return dest_;
        }

    private:
        unsigned short length_;
        unsigned char buffer[1500];
        const char* data;
        struct wifu_packet_header* wifu_header_;
        AddressPort* source_;
        AddressPort* dest_;
    };

    TEST(WiFuPacketTest, EmptyConstructor) {
        WiFuPacket p;
        // TODO: Nothing to test?
    }

    void test_constructor(WiFuPacket & p, WiFuPacketHelper & helper) {
        AddressPort* source = helper.get_source();
        AddressPort* dest = helper.get_dest();
        ASSERT_EQ(source->get_port(), p.get_source_port());
        ASSERT_EQ(dest->get_port(), p.get_destination_port());
        ASSERT_TRUE((*source) == (*p.get_source_address_port()));
        ASSERT_TRUE((*dest) == (*p.get_dest_address_port()));
        ASSERT_TRUE(!strncmp(helper.get_data(), (const char*) p.get_data(), strlen(helper.get_data())));
    }

//    TEST(WiFuPacketTest, BufferConstructor) {
//        WiFuPacketHelper helper;
//        WiFuPacket p(helper.get_buffer(), helper.length());
//        test_constructor(p, helper);
//    }
//
//    TEST(WiFuPacketTest, CopyConstructor) {
//        WiFuPacketHelper helper;
//        WiFuPacket p(helper.get_buffer(), helper.length());
//        WiFuPacket c(p);
//        test_constructor(c, helper);
//    }

    TEST(WiFuPacketTest, SourcePort) {
        WiFuPacket p;
        int port = 5000;
        p.set_source_port(port);
        ASSERT_EQ(port, p.get_source_port());
    }

    TEST(WiFuPacketTest, DestinationPort) {
        WiFuPacket p;
        int port = 5000;
        p.set_destination_port(port);
        ASSERT_EQ(port, p.get_destination_port());
    }

    TEST(WiFuPacketTest, DestinationAddressPort) {
        AddressPort ap("192.168.0.23", 5000);
        WiFuPacket p;
        p.set_ip_destination_address_s(ap.get_address());
        p.set_destination_port(ap.get_port());
        ASSERT_TRUE(ap == (*p.get_dest_address_port()));
    }

    TEST(WiFuPacketTest, SourceAddressPort) {
        AddressPort ap("192.168.0.24", 5001);
        WiFuPacket p;
        p.set_ip_source_address_s(ap.get_address());
        p.set_source_port(ap.get_port());
        ASSERT_TRUE(ap == (*p.get_source_address_port()));
    }

    TEST(WiFuPacketTest, MaxDataSizeTest) {
        WiFuPacket p;
        // MTU - IP header size bytes - WiFu header size bytes
        int exp = 1500 - 20 - 4;

        ASSERT_EQ(exp, p.max_data_length());
    }
}


#endif	/* _WIFUPACKETTEST_H */


