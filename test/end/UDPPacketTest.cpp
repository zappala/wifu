
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

void hexDump (char *desc, void *addr, int len) {
    int i;
    unsigned char buff[17];       // stores the ASCII data
    unsigned char *pc = (unsigned char*)addr;     // cast to make the code cleaner.

    // Output description if given.
    if (desc != NULL)
        printf ("%s:\n", desc);

    // Process every byte in the data.
    for (i = 0; i < len; i++) {

    // Multiple of 16 means new line (with line offset).
        if ((i % 16) == 0) {

        // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
}

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
        UDPPacket p;

        // check against a reference implementation
        // I pulled these values from wireshark - RB
        
        u_int32_t ip_header[5] = {
            ntohl(0x4500004c),
            ntohl(0x00004000),
            ntohl(0xfa119086),
            ntohl(0x0a0b1002),
            ntohl(0xc0a81565)
        };

        memcpy(p.get_payload(), ip_header, sizeof (ip_header));

        u_int32_t udp_header[2] = {
            ntohl(0x007b007b),
            ntohl(0x00381567)
        };


        memcpy(p.get_payload() + sizeof (ip_header), udp_header, sizeof (udp_header));

        u_int32_t data[12] = {
            ntohl(0x1c0106ef),
            ntohl(0x00000001),
            ntohl(0x00000001),
            ntohl(0x47505320),
            ntohl(0xd300eb91),
            ntohl(0x00000000),
            ntohl(0xd300eb92),
            ntohl(0x108bd7f5),
            ntohl(0xd300eb92),
            ntohl(0x10a3d70a),
            ntohl(0xd300eb92),
            ntohl(0x10e56042)            
        };

        memcpy(p.get_payload() + sizeof (ip_header) + sizeof(udp_header), data, sizeof (data));

        // test multiple times to ensure that doing one doesn't mess up the other
        ASSERT_TRUE(p.is_valid_ip_checksum());
        ASSERT_TRUE(p.is_valid_udp_checksum());
        ASSERT_TRUE(p.is_valid_ip_checksum());
        ASSERT_TRUE(p.is_valid_udp_checksum());
    }

    TEST(UDPPacketTest, MaxDataSizeTest) {
        UDPPacket* p = new UDPPacket();
        // MTU - IP header size bytes - UDP header size bytes
        int exp = 1500 - 20 - 8;

        ASSERT_EQ(exp, p->max_data_length());
    }
}

#endif	/* _UDPPACKETTEST_H */


