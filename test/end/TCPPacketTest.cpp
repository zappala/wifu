/*
 * File:   TCPPacketTest.h
 * Author: rbuck
 *
 * Created on January 7, 2011, 3:03 PM
 */

#ifndef _TCPPACKETTEST_H
#define	_TCPPACKETTEST_H

#include <iostream>

#include "gtest/gtest.h"
#include "packet/TCPPacket.h"
#include "RandomNumberSet.h"
#include "packet/TCPTimestampOption.h"

#include <netinet/tcp.h>

using namespace std;

namespace {

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
            tcp_header_->ack_seq = htonl(2);
            tcp_header_->check = 3;
            tcp_header_->dest = htons(dest_->get_port());
            tcp_header_->doff = (sizeof (struct tcphdr) / 4) + 3;
            tcp_header_->fin = 1;
            tcp_header_->psh = 1;
            //            tcp_header_->res1 = 1;
            //            tcp_header_->res2 = 1;
            tcp_header_->rst = 1;
            tcp_header_->seq = htonl(4);
            tcp_header_->source = htons(source_->get_port());
            tcp_header_->syn = 1;
            tcp_header_->urg = 1;
            tcp_header_->urg_ptr = htons(6);
            tcp_header_->window = htons(7);

            options_ = (unsigned char*) buffer + (sizeof (struct iphdr) + sizeof (struct tcphdr));
            u_int8_t kind = TCPOPT_TIMESTAMP;
            u_int8_t length = TCPOLEN_TIMESTAMP;
            struct wifu_tcp_timestamp ts;
            ts.timestamp_echo_reply_ = 4;
            ts.timestamp_value_ = 5;
            memcpy(options_, &kind, 1);
            memcpy(options_ + 1, &length, 1);
            memcpy(options_ + 2, &ts, sizeof (ts));
            memset(options_ + 2 + sizeof (ts), 0, 2);

            unsigned char* payload = options_ + 12;
            memcpy(payload, data, strlen(data));

            length_ = strlen(data) + sizeof (struct iphdr) + sizeof (struct tcphdr) + 12;
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
        unsigned char* options_;
        AddressPort* source_;
        AddressPort* dest_;
    };

    TEST(TCPPacketTest, EmptyConstructor) {
        TCPPacket p;
        // TODO: Nothing to test?
    }

    void test_helper(TCPPacket & p, TCPPacketHelper & helper) {
        AddressPort* source = helper.get_source();
        AddressPort* dest = helper.get_dest();

        ASSERT_EQ(1, p.is_tcp_ack());
        ASSERT_EQ(2, p.get_tcp_ack_number());
        ASSERT_EQ(3, p.get_tcp_checksum());
        ASSERT_EQ(dest->get_port(), p.get_destination_port());
        // Add 3 for number of 32 bit words in the options
        ASSERT_EQ((sizeof (struct tcphdr) / 4) + 3, p.get_tcp_data_offset());
        // Add 12 for the number of bytes in the options
        ASSERT_EQ(sizeof (struct tcphdr) + 12, p.get_tcp_header_length_bytes());
        // Options
        TCPTimestampOption* option = (TCPTimestampOption*) p.get_option(TCPOPT_TIMESTAMP);
        ASSERT_TRUE(option != 0);
        ASSERT_EQ(TCPOPT_TIMESTAMP, option->get_kind());
        ASSERT_EQ(TCPOLEN_TIMESTAMP, option->get_length());
        ASSERT_EQ(5, option->get_timestamp());
        ASSERT_EQ(4, option->get_echo_reply());
        // Option padding
        ASSERT_EQ(0, *(option->get_data() + option->get_length()));
        ASSERT_EQ(0, *(option->get_data() + option->get_length() + 1));

        ASSERT_EQ(1, p.is_tcp_fin());
        ASSERT_EQ(1, p.is_tcp_psh());
        ASSERT_EQ(1, p.is_tcp_rst());
        ASSERT_EQ(4, p.get_tcp_sequence_number());
        ASSERT_EQ(source->get_port(), p.get_source_port());
        ASSERT_EQ(1, p.is_tcp_syn());
        ASSERT_EQ(1, p.is_tcp_urg());
        ASSERT_EQ(6, p.get_tcp_urgent_pointer());
        ASSERT_EQ(7, p.get_tcp_receive_window_size());
        ASSERT_TRUE(!strncmp(helper.get_data(), (const char*) p.get_data(), strlen(helper.get_data())));
        ASSERT_TRUE((*source) == (*p.get_source_address_port()));
        ASSERT_TRUE((*dest) == (*p.get_dest_address_port()));
    }

    TEST(TCPPacketTest, SequenceNumberTest) {
        RandomNumberSet<u_int32_t> random;
        TCPPacket p;

        u_int32_t value = random.get();
        p.set_tcp_sequence_number(value);
        ASSERT_EQ(value, p.get_tcp_sequence_number());
    }

    TEST(TCPPacketTest, AckNumberTest) {
        RandomNumberSet<u_int32_t> random;
        TCPPacket p;

        u_int32_t value = random.get();
        p.set_tcp_ack_number(value);
        ASSERT_EQ(value, p.get_tcp_ack_number());
    }

    TEST(TCPPacketTest, TCPHeaderLengthBytesTest) {
        TCPPacket p;
        ASSERT_EQ(sizeof (struct tcphdr), p.get_tcp_header_length_bytes());
    }

    TEST(TCPPacketTest, TCPHeaderLengthTest) {
        TCPPacket p;
        ASSERT_EQ(sizeof (struct tcphdr) / 4, p.get_tcp_data_offset());
    }

    TEST(TCPPacketTest, URG) {
        TCPPacket p;

        p.set_tcp_urg(true);
        ASSERT_TRUE(p.is_tcp_urg());

        p.set_tcp_urg(false);
        ASSERT_TRUE(!p.is_tcp_urg());
    }

    TEST(TCPPacketTest, ACK) {
        TCPPacket p;

        p.set_tcp_ack(true);
        ASSERT_TRUE(p.is_tcp_ack());

        p.set_tcp_ack(false);
        ASSERT_TRUE(!p.is_tcp_ack());
    }

    TEST(TCPPacketTest, PSH) {
        TCPPacket p;

        p.set_tcp_psh(true);
        ASSERT_TRUE(p.is_tcp_psh());

        p.set_tcp_psh(false);
        ASSERT_TRUE(!p.is_tcp_psh());
    }

    TEST(TCPPacketTest, RST) {
        TCPPacket p;

        p.set_tcp_rst(true);
        ASSERT_TRUE(p.is_tcp_rst());

        p.set_tcp_rst(false);
        ASSERT_TRUE(!p.is_tcp_rst());
    }

    TEST(TCPPacketTest, SYN) {
        TCPPacket p;

        p.set_tcp_syn(true);
        ASSERT_TRUE(p.is_tcp_syn());

        p.set_tcp_syn(false);
        ASSERT_TRUE(!p.is_tcp_syn());
    }

    TEST(TCPPacketTest, FIN) {
        TCPPacket p;

        p.set_tcp_fin(true);
        ASSERT_TRUE(p.is_tcp_fin());

        p.set_tcp_fin(false);
        ASSERT_TRUE(!p.is_tcp_fin());
    }

    TEST(TCPPacketTest, ReceiveWindowTest) {
        RandomNumberSet<u_int16_t> random;
        TCPPacket p;

        u_int16_t value = random.get();
        p.set_tcp_receive_window_size(value);
        ASSERT_EQ(value, p.get_tcp_receive_window_size());
    }

    TEST(TCPPacketTest, ChecksumTest) {
        RandomNumberSet<u_int16_t> random;
        TCPPacket p;

        u_int16_t value = random.get();
        p.set_tcp_checksum(value);
        ASSERT_EQ(value, p.get_tcp_checksum());
    }

    TEST(TCPPacketTest, UrgentPointerTest) {
        RandomNumberSet<u_int16_t> random;
        TCPPacket p;

        u_int16_t value = random.get();
        p.set_tcp_urgent_pointer(value);
        ASSERT_EQ(value, p.get_tcp_urgent_pointer());
    }

    TEST(TCPPacketTest, MaxDataSizeTest) {
        TCPPacket p;
        // MTU - IP header size bytes - TCP header size bytes
        int exp = 1500 - 20 - 20;

        ASSERT_EQ(exp, p.max_data_length());
    }

    // Options

    TEST(TCPPacketTest, AddRemoveTCPHeaderOptions) {
        TCPPacket p;

        TCPHeaderOption* expected = 0;
        TCPHeaderOption* actual = p.remove_tcp_header_option(TCPOPT_TIMESTAMP);
        EXPECT_EQ(expected, actual);

        expected = new TCPTimestampOption();
        p.insert_tcp_header_option(expected);
        actual = p.remove_tcp_header_option(expected->get_kind());
        EXPECT_EQ(expected, actual);

        actual = p.remove_tcp_header_option(expected->get_kind());
        EXPECT_EQ(0, actual);
    }

    TEST(TCPPacketTest, AddTCPTimestampHeaderOption) {
        u_int32_t timestamp = 5;
        u_int32_t echo = 4;

        TCPPacket p;

        p.set_ip_checksum(1);
        p.set_ip_destination_address_s("192.168.0.2");
        p.set_ip_fragmentation_offset(3);
        p.set_ip_identifier(4);
        p.set_ip_header_length_words(5);
        p.set_ip_protocol(6);
        p.set_ip_source_address_s("192.168.0.1");
        p.set_ip_tos(8);
        p.set_ip_ttl(10);
        p.set_ip_version(11);

        p.set_tcp_ack(1);
        p.set_tcp_ack_number(2);
        p.set_tcp_checksum(3);
        p.set_destination_port(5002);
        p.set_tcp_fin(1);
        p.set_tcp_psh(1);
        p.set_tcp_rst(1);
        p.set_tcp_sequence_number(4);
        p.set_source_port(5001);
        p.set_tcp_syn(1);
        p.set_tcp_urg(1);
        p.set_tcp_urgent_pointer(6);
        p.set_tcp_receive_window_size(7);

        TCPTimestampOption* option = new TCPTimestampOption();
        option->set_echo_reply(echo);
        option->set_timestamp(timestamp);
        p.insert_tcp_header_option(option);

        const char* data = "This is some cool data";
        p.set_data((unsigned char*) data, strlen(data));
        p.pack();

        TCPPacketHelper helper;
        test_helper(p, helper);

        // Copy payload to another packet to ensure we can read/parse the option correctly
        TCPPacket other;
        memcpy(other.get_payload(), p.get_payload(), MTU);
        test_helper(other, helper);
    }
}


#endif	/* _TCPPACKETTEST_H */


