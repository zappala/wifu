/* 
 * File:   TCPPacketTest.h
 * Author: rbuck
 *
 * Created on January 7, 2011, 3:03 PM
 */

#ifndef _TCPPACKETTEST_H
#define	_TCPPACKETTEST_H

#include <iostream>

#include "UnitTest++.h"
#include "../headers/packet/TCPPacket.h"
#include "../headers/RandomNumberSet.h"

using namespace std;

namespace {

    SUITE(TCPPacket) {

        class TCPPacketHelper {
        public:

            TCPPacketHelper() : data("This is some cool data") {
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

                tcp_header_ = (struct tcphdr*) (buffer + sizeof (struct iphdr));
                tcp_header_->ack = 1;
                tcp_header_->ack_seq = 2;
                tcp_header_->check = 3;
                tcp_header_->dest = dest_->get_port();
                tcp_header_->doff = sizeof (struct tcphdr) / 4;
                tcp_header_->fin = 1;
                tcp_header_->psh = 1;
                tcp_header_->res1 = 1;
                tcp_header_->res2 = 1;
                tcp_header_->rst = 1;
                tcp_header_->seq = 4;
                tcp_header_->source = source_->get_port();
                tcp_header_->syn = 1;
                tcp_header_->urg = 1;
                tcp_header_->urg_ptr = 6;
                tcp_header_->window = 7;

                unsigned char* payload = buffer + (sizeof (struct iphdr) + sizeof (struct tcphdr));
                memcpy(payload, data, strlen(data));

                length_ = strlen(data) + sizeof (struct iphdr) + sizeof (struct tcphdr);
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

            struct tcphdr* get_tcp_header() {
                return tcp_header_;
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
            struct tcphdr* tcp_header_;
            AddressPort* source_;
            AddressPort* dest_;
        };

        TEST(EmptyConstructor) {
            TCPPacket p;
            // TODO: Nothing to test?
        }

        void test_constructor(TCPPacket & p, TCPPacketHelper & helper) {
            AddressPort* source = helper.get_source();
            AddressPort* dest = helper.get_dest();

            CHECK_EQUAL(1, p.is_tcp_ack());
            CHECK_EQUAL(2, p.get_tcp_ack_number());
            CHECK_EQUAL(3, p.get_tcp_checksum());
            CHECK_EQUAL(dest->get_port(), p.get_destination_port());
            CHECK_EQUAL(sizeof (struct tcphdr) / 4, p.get_tcp_header_length());
            CHECK_EQUAL(sizeof (struct tcphdr), p.get_tcp_header_length_bytes());
            CHECK_EQUAL(1, p.is_tcp_fin());
            CHECK_EQUAL(1, p.is_tcp_psh());
            CHECK_EQUAL(1, p.is_tcp_rst());
            CHECK_EQUAL(4, p.get_tcp_sequence_number());
            CHECK_EQUAL(source->get_port(), p.get_source_port());
            CHECK_EQUAL(1, p.is_tcp_syn());
            CHECK_EQUAL(1, p.is_tcp_urg());
            CHECK_EQUAL(6, p.get_tcp_urgent_pointer());
            CHECK_EQUAL(7, p.get_tcp_receive_window_size());
            CHECK(!strncmp(helper.get_data(), (const char*) p.get_data(), strlen(helper.get_data())));
            CHECK((*source) == (*p.get_source_address_port()));
            CHECK((*dest) == (*p.get_dest_address_port()));
        }

        TEST(BufferConstructor) {
            TCPPacketHelper helper;
            TCPPacket p(helper.get_buffer(), helper.length());
            test_constructor(p, helper);
        }

        TEST(CopyConstructor) {
            TCPPacketHelper helper;
            TCPPacket p(helper.get_buffer(), helper.length());
            TCPPacket c(p);
            test_constructor(c, helper);
        }

        TEST(SequenceNumberTest) {
            RandomNumberSet<u_int32_t> random;
            TCPPacket p;

            u_int32_t value = random.get();
            p.set_tcp_sequence_number(value);
            CHECK_EQUAL(value, p.get_tcp_sequence_number());
        }

        TEST(AckNumberTest) {
            RandomNumberSet<u_int32_t> random;
            TCPPacket p;

            u_int32_t value = random.get();
            p.set_tcp_ack_number(value);
            CHECK_EQUAL(value, p.get_tcp_ack_number());
        }

        TEST(TCPHeaderLengthBytesTest) {
            TCPPacket p;
            CHECK_EQUAL(sizeof (struct tcphdr), p.get_tcp_header_length_bytes());
        }

        TEST(TCPHeaderLengthTest) {
            TCPPacket p;
            CHECK_EQUAL(sizeof (struct tcphdr) / 4, p.get_tcp_header_length());
        }

        TEST(URG) {
            TCPPacket p;

            p.set_tcp_urg(true);
            CHECK(p.is_tcp_urg());

            p.set_tcp_urg(false);
            CHECK(!p.is_tcp_urg());
        }

        TEST(ACK) {
            TCPPacket p;

            p.set_tcp_ack(true);
            CHECK(p.is_tcp_ack());

            p.set_tcp_ack(false);
            CHECK(!p.is_tcp_ack());
        }

        TEST(PSH) {
            TCPPacket p;

            p.set_tcp_psh(true);
            CHECK(p.is_tcp_psh());

            p.set_tcp_psh(false);
            CHECK(!p.is_tcp_psh());
        }

        TEST(RST) {
            TCPPacket p;

            p.set_tcp_rst(true);
            CHECK(p.is_tcp_rst());

            p.set_tcp_rst(false);
            CHECK(!p.is_tcp_rst());
        }

        TEST(SYN) {
            TCPPacket p;

            p.set_tcp_syn(true);
            CHECK(p.is_tcp_syn());

            p.set_tcp_syn(false);
            CHECK(!p.is_tcp_syn());
        }

        TEST(FIN) {
            TCPPacket p;

            p.set_tcp_fin(true);
            CHECK(p.is_tcp_fin());

            p.set_tcp_fin(false);
            CHECK(!p.is_tcp_fin());
        }

        TEST(ReceiveWindowTest) {
            RandomNumberSet<u_int16_t> random;
            TCPPacket p;

            u_int16_t value = random.get();
            p.set_tcp_receive_window_size(value);
            CHECK_EQUAL(value, p.get_tcp_receive_window_size());
        }

        TEST(ChecksumTest) {
            RandomNumberSet<u_int16_t> random;
            TCPPacket p;

            u_int16_t value = random.get();
            p.set_tcp_checksum(value);
            CHECK_EQUAL(value, p.get_tcp_checksum());
        }

        TEST(UrgentPointerTest) {
            RandomNumberSet<u_int16_t> random;
            TCPPacket p;

            u_int16_t value = random.get();
            p.set_tcp_urgent_pointer(value);
            CHECK_EQUAL(value, p.get_tcp_urgent_pointer());
        }
    }
}


#endif	/* _TCPPACKETTEST_H */


