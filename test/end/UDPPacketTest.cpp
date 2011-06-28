
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
        RandomNumberSet<u_int16_t>* random = new RandomNumberSet<u_int16_t>();
        UDPPacket p = UDPPacket();

        u_int16_t value = random->get();
        p.set_udp_checksum(value);
        ASSERT_EQ(value, p.get_udp_checksum());

        /* Our fine example UDP packet from a wired shark (Wireshark):
0000  ff ff ff ff ff ff 00 0f  fe fe 54 37 08 00 45 00   ........ ..T7..E.
0010  00 2a 00 00 40 00 40 11  64 b4 c0 a8 15 67 ff ff   .*..@.@. d....g..
0020  ff ff b7 37 13 88 00 16  d6 36 5a 54 20 31 2e 30   ...7.... .6ZT 1.0
0030  20 66 69 6c 65 73 3f 0a                             files?.
         */
        //Note: checksum was 0xED87
        //Bad packet off the wire; source was Python, look for the error there

//        u_int32_t ip_header[5] = {
//            ntohl(0x4500002a),
//            ntohl(0x00004000),
//            ntohl(0x401164b4),
//            ntohl(0xc0a81567),
//            ntohl(0xffffffff)
//        };
//
//        memcpy(p.get_payload(), ip_header, sizeof (ip_header));
//
//        u_int32_t udp_header[2] = {
//            ntohl(0xb7371388),
//            ntohl(0x0016d636)
//        };
//
//        memcpy(p.get_payload() + sizeof (ip_header), udp_header, sizeof (udp_header));
//
//        //headers are in, put in data
//
//        /*u_int32_t data[4] = {
//            ntohl(0x5a542031),
//            ntohl(0x2e302066),
//            ntohl(0x696c6573),
//            ntohl(0x3f0a0000)
//        };*/
//
//        u_int16_t data[7] = {
//            ntohs(0x5a54),
//            ntohs(0x2031),
//            ntohs(0x2e30),
//            ntohs(0x2066),
//            ntohs(0x696c),
//            ntohs(0x6573),
//            ntohs(0x3f0a)
////            0x5a54,
////            0x2031,
////            0x2e30,
////            0x2066,
////            0x696c,
////            0x6573,
////            0x3f0a
//        };

/*
0000  ff ff ff ff ff ff 78 e7  d1 85 8a f5 08 00 45 00   ......x. ......E.
0010  00 b7 00 00 40 00 40 11  64 26 c0 a8 15 68 ff ff   ....@.@. d&...h..
0020  ff ff 44 5c 44 5c 00 a3  89 6f 7b 22 68 6f 73 74   ..D\D\.. .o{"host
0030  5f 69 6e 74 22 3a 20 35  39 31 30 38 35 33 33 2c   _int": 5 9108533,
0040  20 22 76 65 72 73 69 6f  6e 22 3a 20 5b 31 2c 20    "versio n": [1,
0050  38 5d 2c 20 22 64 69 73  70 6c 61 79 6e 61 6d 65   8], "dis playname
0060  22 3a 20 22 35 39 31 30  38 35 33 33 22 2c 20 22   ": "5910 8533", "
0070  70 6f 72 74 22 3a 20 31  37 35 30 30 2c 20 22 6e   port": 1 7500, "n
0080  61 6d 65 73 70 61 63 65  73 22 3a 20 5b 32 38 31   amespace s": [281
0090  34 36 32 37 39 2c 20 34  34 30 33 39 34 34 2c 20   46279, 4 403944,
00a0  32 37 35 35 33 39 39 2c  20 33 33 39 36 30 33 33   2755399,  3396033
00b0  32 2c 20 34 36 33 39 31  39 35 2c 20 32 37 35 35   2, 46391 95, 2755
00c0  33 35 39 5d 7d                                     359]}
 Checksum should be: 0x896f
 */
        u_int32_t ip_header[5] = {
            ntohl(0x450000b7),
            ntohl(0x00004000),
            ntohl(0x40116426),
            ntohl(0xc0a81568),
            ntohl(0xffffffff)
        };

        memcpy(p.get_payload(), ip_header, sizeof (ip_header));


        u_int32_t udp_header[2] = {
            ntohl(0x445c445c),
            ntohl(0x00a3896f)
        };

        memcpy(p.get_payload() + sizeof (ip_header), udp_header, sizeof (udp_header));

        //headers are in, put in data

        /*u_int32_t data[4] = {
            ntohl(0x5a542031),
            ntohl(0x2e302066),
            ntohl(0x696c6573),
            ntohl(0x3f0a0000)
        };*/

        u_int32_t data[39] = {
           ntohl(0x7b22686f),
           ntohl(0x73745f69),
           ntohl(0x6e74223a),
           ntohl(0x20353931),
           ntohl(0x30383533),
           ntohl(0x332c2022),
           ntohl(0x76657273),
           ntohl(0x696f6e22),
           ntohl(0x3a205b31),
           ntohl(0x2c20385d),
           ntohl(0x2c202264),
           ntohl(0x6973706c),
           ntohl(0x61796e61),
           ntohl(0x6d65223a),
           ntohl(0x20223539),
           ntohl(0x31303835),
           ntohl(0x3333222c),
           ntohl(0x2022706f),
           ntohl(0x7274223a),
           ntohl(0x20313735),
           ntohl(0x30302c20),
           ntohl(0x226e616d),
           ntohl(0x65737061),
           ntohl(0x63657322),
           ntohl(0x3a205b32),
           ntohl(0x38313436),
           ntohl(0x3237392c),
           ntohl(0x20343430),
           ntohl(0x33393434),
           ntohl(0x2c203237),
           ntohl(0x35353339),
           ntohl(0x392c2033),
           ntohl(0x33393630),
           ntohl(0x3333322c),
           ntohl(0x20343633),
           ntohl(0x39313935),
           ntohl(0x2c203237),
           ntohl(0x35353335),
           ntohl(0x395d7d00)
        };

        //cout << "Data: " << data[0] << endl;
        //p.set_udp_length_bytes(8);
        //p.set_ip_tot_length(p.get_ip_tot_length() - 14);
        //p.calculate_and_set_ip_checksum();

        memcpy(p.get_payload() + sizeof (ip_header) + sizeof(udp_header), data, sizeof (data));

        unsigned char* payload = p.get_payload();
//        cout << "Hex dump:\n";
//        hexDump (NULL, payload, sizeof(ip_header) + sizeof(udp_header) + sizeof(data));

//        cout << "Checksum was: " << p.get_udp_checksum() << "\n";
//
//        cout << "Packet contents: \n" << p.to_s() << "\n";
//
//        cout << "Data: \n" << p.get_data() << "\n";

        // test multiple times to ensure that doing one doesn't mess up the other
        ASSERT_TRUE(p.is_valid_ip_checksum());
        ASSERT_TRUE(p.is_valid_udp_checksum());
        ASSERT_TRUE(p.is_valid_ip_checksum());
        ASSERT_TRUE(p.is_valid_udp_checksum());

        // change sport
        u_int16_t sport = p.get_source_port();
        p.set_source_port(13);
        ASSERT_FALSE(p.is_valid_udp_checksum());
        p.set_source_port(sport);
        ASSERT_TRUE(p.is_valid_udp_checksum());

        // change dport
        u_int16_t dport = p.get_destination_port();
        p.set_destination_port(13);
        ASSERT_FALSE(p.is_valid_udp_checksum());
        p.set_destination_port(dport);
        ASSERT_TRUE(p.is_valid_udp_checksum());

        //p->set_udp_checksum(0);
        //p->set_data(hi, test.length());
        //p->calculate_and_set_udp_checksum();
        //cout << "Checksum was: " << p->get_udp_checksum() << "\n";
    }

    TEST(UDPPacketTest, MaxDataSizeTest) {
        UDPPacket* p = new UDPPacket();
        // MTU - IP header size bytes - UDP header size bytes
        int exp = 1500 - 20 - 8;

        ASSERT_EQ(exp, p->max_data_length());
    }
}


#endif	/* _UDPPACKETTEST_H */


