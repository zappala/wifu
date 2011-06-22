
/*
 * File:   UDPPacketTest.h
 * Author: leer
 *
 * Created on June 22, 2011, 11:45 AM
 */

#ifndef _UDPPACKETTEST_H
#define	_UDPPACKETTEST_H

#include <iostream>

#include "gtest/gtest.h"
#include "packet/UDPPacket.h"
#include "RandomNumberSet.h"

#include <netinet/udp.h>

using namespace std;

namespace {

    class UDPPacketHelper {
    public:

        UDPPacketHelper() : data("This is some cool data") {
            source_ = new AddressPort("192.168.0.1", 5001);
            dest_ = new AddressPort("192.168.0.2", 5002);

            udp_header_ = (struct udphdr*) (buffer + sizeof (struct iphdr));
            udp_header_->check = 3;
            udp_header_->dest = htons(dest_->get_port());
            udp_header_->source = htons(source_->get_port());

            length_ = strlen(data) + sizeof (struct iphdr) + sizeof (struct udphdr) + 12;
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

        struct udphdr* get_udp_header() {
            return udp_header_;
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
        struct udphdr* udp_header_;
        AddressPort* source_;
        AddressPort* dest_;
    };

    TEST(UDPPacketTest, EmptyConstructor) {
        UDPPacket p;
        // TODO: Nothing to test?
    }

    void test_helper(UDPPacket & p, UDPPacketHelper & helper) {
        AddressPort* source = helper.get_source();
        AddressPort* dest = helper.get_dest();

        ASSERT_EQ(3, p.get_udp_checksum());
        ASSERT_EQ(dest->get_port(), p.get_destination_port());
        //ASSERT_EQ((sizeof (struct udphdr) / 4), p.get_data());
        ASSERT_EQ(sizeof (struct udphdr), UDP_HEADER_LENGTH_BYTES);
        
        ASSERT_TRUE(!strncmp(helper.get_data(), (const char*) p.get_data(), strlen(helper.get_data())));
        ASSERT_TRUE((*source) == (*p.get_source_address_port()));
        ASSERT_TRUE((*dest) == (*p.get_dest_address_port()));
    }

    TEST(UDPPacketTest, UDPHeaderLengthBytesTest) {
        UDPPacket p;
        ASSERT_EQ(sizeof (struct udphdr), UDP_HEADER_LENGTH_BYTES);
    }

    TEST(UDPPacketTest, ChecksumTest) {
        RandomNumberSet<u_int16_t>* random = new RandomNumberSet<u_int16_t>();
        UDPPacket* p = new UDPPacket();

        u_int16_t value = random->get();
        cout << "Not segfaulting in the test; your UDP packet sucks!\n";
        p->set_udp_checksum(value);
        ASSERT_EQ(value, p->get_udp_checksum());
    }

    TEST(UDPPacketTest, MaxDataSizeTest) {
        UDPPacket* p = new UDPPacket();
        // MTU - IP header size bytes - UDP header size bytes
        int exp = 1500 - 20 - 8;

        ASSERT_EQ(exp, p->max_data_length());
    }
}


#endif	/* _UDPPACKETTEST_H */


