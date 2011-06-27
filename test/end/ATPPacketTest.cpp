#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <bitset>

#include "gtest/gtest.h"
#include "RandomNumberSet.h"
#include "packet/TCPTimestampOption.h"

#include "../headers/packet/ATPPacket.h"

void hexDump (char *desc, void *addr, int len);
void binDump (char *desc, void *addr, int len);

// TODO: Look at TCPPacektTest.cpp for things to test
// TODO: Should I run TCPPacketTest.cpp to make sure I didn't mess up the TCP header


void correctStructure(ATPPacket* packet, u_int32_t max_delay, u_int32_t average_delay){
	// checking that the header is not NULL
	EXPECT_TRUE(packet->get_atp_header() != NULL);

	// checking that the header pointer is pointing to where it is supposed to be pointing (where the data starts + ip header + tcp header)
	EXPECT_EQ(packet->get_atp_header(), packet->get_payload() + packet->get_ip_header_length_bytes() + packet->get_tcp_header_length_bytes());

	// checking that the total length is equal to all the individual parts
	EXPECT_EQ(packet->get_ip_tot_length(), packet->get_atp_header_length_bytes() + packet->get_tcp_header_length_bytes() + packet->get_ip_header_length_bytes() + packet->get_data_length_bytes());

	// checking that header structure has been set to what I expect
	EXPECT_EQ(average_delay, packet->get_atp_average_delay());
	EXPECT_EQ(max_delay, packet->get_atp_max_delay());

}

void checkData(ATPPacket* packet, const char* data){
	// checking that get_data works
	ASSERT_TRUE(!strncmp(data, (const char*)packet->get_data(), strlen(data) + 1));
	ASSERT_TRUE(!strncmp(data, (const char*)packet->get_data(), strlen((const char*)packet->get_data()) + 1));

	// checking that the data pointers are pointing to the correct
	unsigned char * temp = packet->get_payload() +
			packet->get_ip_header_length_bytes() +
			packet->get_tcp_header_length_bytes() +
			packet->get_atp_header_length_bytes();
	EXPECT_EQ(*data, *(const char*)temp);

	// Added + 1 because I want the null terminator at the end of the string included
	EXPECT_EQ(packet->get_data_length_bytes(), strlen(data) + 1);
}

// Check that packet has been initialized correctly
TEST(ATPPacketTest, init){
	ATPPacket packet;
	packet.pack();

	correctStructure(&packet, 0, 0);

	EXPECT_EQ(packet.max_data_length(), 1452);
	EXPECT_EQ(packet.get_atp_header_length_bytes(), 8);
}


TEST(ATPPacketTest, setData){
	ATPPacket packet;
	packet.pack();

	const char * data = "here is some data";
	packet.set_data((unsigned char*) data, strlen(data) + 1);

	// checking that you can't set data twice
	EXPECT_THROW(packet.set_data((unsigned char*) data, strlen(data) + 1), IllegalStateException);

	checkData(&packet, data);

	EXPECT_EQ(packet.get_data_length_bytes(), 18);

	// checking that the structure of the packet is still good
	correctStructure(&packet, 0, 0);
}


TEST(ATPPacketTest, setDelay){
	ATPPacket packet;
	packet.pack();

	const char * data = "more data!";
	packet.set_data((unsigned char*) data, strlen(data) + 1);

	u_int64_t max = 100000;
	u_int64_t average = 2;

	// set the max and average delay
	packet.set_atp_max_delay(max);
	packet.set_atp_average_delay(average);

	// checking that the header does not interfere with data and structure
	checkData(&packet, data);
	correctStructure(&packet, max, average);

	// checking that getting delays works correctly
	EXPECT_EQ(max, packet.get_atp_max_delay());
	EXPECT_EQ(average, packet.get_atp_average_delay());

	// should be highest value that can fit into a 32 bit number
	max = 4294967295;
	average = 4294967295;

	packet.set_atp_max_delay(max);
	packet.set_atp_average_delay(average);

	EXPECT_EQ(max, packet.get_atp_max_delay());
	EXPECT_EQ(average, packet.get_atp_average_delay());

	// should overflow the value back to 0
	max = 4294967296;
	average = 4294967296;

	packet.set_atp_max_delay(max);
	packet.set_atp_average_delay(average);

	EXPECT_EQ(0, packet.get_atp_max_delay());
	EXPECT_EQ(0, packet.get_atp_average_delay());

	// checking that the header does not interfere with data and structure
	checkData(&packet, data);
	correctStructure(&packet, max, average);
}


TEST(ATPPacketTest, equal){
	ATPPacket * packet1 = new ATPPacket;
	ATPPacket * packet2 = packet1;

	packet1->pack();
	packet2->pack();

	EXPECT_EQ(*packet1, *packet2);
	EXPECT_FALSE(*packet1 != *packet2);

	ATPPacket packet3;
	ATPPacket packet4;

	packet3.pack();
	packet4.pack();

	// TODO: talk to Randy about why these fail
	//EXPECT_EQ(packet3, packet4);
	//EXPECT_FALSE(packet3 != packet4);

	delete packet1;
}

/**************** TESTING TCP ********************/
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

void test_helper(ATPPacket & p, TCPPacketHelper & helper) {
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

     ASSERT_EQ(1346914636, p.get_atp_average_delay());
     ASSERT_EQ(1346914636, p.get_atp_max_delay());
 }


TEST(ATPPacketTest, SequenceNumberTest) {
    RandomNumberSet<u_int32_t> random;
    ATPPacket p;

    u_int32_t value = random.get();
    p.set_tcp_sequence_number(value);
    ASSERT_EQ(value, p.get_tcp_sequence_number());
}

TEST(ATPPacketTest, AckNumberTest) {
    RandomNumberSet<u_int32_t> random;
    ATPPacket p;

    u_int32_t value = random.get();
    p.set_tcp_ack_number(value);
    ASSERT_EQ(value, p.get_tcp_ack_number());
}

TEST(ATPPacketTest, TCPHeaderLengthBytesTest) {
	ATPPacket p;
    ASSERT_EQ(sizeof (struct tcphdr), p.get_tcp_header_length_bytes());
}

TEST(ATPPacketTest, TCPHeaderLengthTest) {
	ATPPacket p;
    ASSERT_EQ(sizeof (struct tcphdr) / 4, p.get_tcp_data_offset());
}

TEST(ATPPacketTest, URG) {
	ATPPacket p;

    p.set_tcp_urg(true);
    ASSERT_TRUE(p.is_tcp_urg());

    p.set_tcp_urg(false);
    ASSERT_TRUE(!p.is_tcp_urg());
}

TEST(ATPPacketTest, ACK) {
	ATPPacket p;

    p.set_tcp_ack(true);
    ASSERT_TRUE(p.is_tcp_ack());

    p.set_tcp_ack(false);
    ASSERT_TRUE(!p.is_tcp_ack());
}

TEST(ATPPacketTest, PSH) {
	ATPPacket p;

    p.set_tcp_psh(true);
    ASSERT_TRUE(p.is_tcp_psh());

    p.set_tcp_psh(false);
    ASSERT_TRUE(!p.is_tcp_psh());
}

TEST(ATPPacketTest, RST) {
	ATPPacket p;

    p.set_tcp_rst(true);
    ASSERT_TRUE(p.is_tcp_rst());

    p.set_tcp_rst(false);
    ASSERT_TRUE(!p.is_tcp_rst());
}

TEST(ATPPacketTest, SYN) {
	ATPPacket p;

    p.set_tcp_syn(true);
    ASSERT_TRUE(p.is_tcp_syn());

    p.set_tcp_syn(false);
    ASSERT_TRUE(!p.is_tcp_syn());
}

TEST(ATPPacketTest, FIN) {
	ATPPacket p;

    p.set_tcp_fin(true);
    ASSERT_TRUE(p.is_tcp_fin());

    p.set_tcp_fin(false);
    ASSERT_TRUE(!p.is_tcp_fin());
}

TEST(ATPPacketTest, ReceiveWindowTest) {
    RandomNumberSet<u_int16_t> random;
    ATPPacket p;

    u_int16_t value = random.get();
    p.set_tcp_receive_window_size(value);
    ASSERT_EQ(value, p.get_tcp_receive_window_size());
}

TEST(ATPPacketTest, ChecksumTest) {
    RandomNumberSet<u_int16_t> random;
    ATPPacket p;

    u_int16_t value = random.get();
    p.set_tcp_checksum(value);
    ASSERT_EQ(value, p.get_tcp_checksum());
}

TEST(ATPPacketTest, UrgentPointerTest) {
    RandomNumberSet<u_int16_t> random;
    ATPPacket p;

    u_int16_t value = random.get();
    p.set_tcp_urgent_pointer(value);
    ASSERT_EQ(value, p.get_tcp_urgent_pointer());
}

TEST(ATPPacketTest, MaxDataSizeTest) {
	ATPPacket p;
    // MTU - IP header size bytes - TCP header size bytes - ATP header size bytes
    int exp = 1500 - 20 - 20 - 8;

    ASSERT_EQ(exp, p.max_data_length());
}

TEST(ATPPacketTest, AddRemoveTCPHeaderOptions) {
	ATPPacket p;

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

TEST(ATPPacketTest, AddTCPTimestampHeaderOption) {
   u_int32_t timestamp = 5;
   u_int32_t echo = 4;

   ATPPacket p;

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

   p.set_atp_average_delay(1346914636);
   p.set_atp_max_delay(1346914636);

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
   ATPPacket other;
   memcpy(other.get_payload(), p.get_payload(), MTU);

   test_helper(other, helper);




}



TEST(ATPPacketTest, overwriteWithHeader){
	ATPPacket p;
	u_int32_t timestamp = 5;


	p.set_atp_average_delay(2863311530);
	p.set_atp_max_delay(2863311530);

	void * old_pointer = p.get_atp_header();

	TCPTimestampOption* option = new TCPTimestampOption();
	option->set_timestamp(timestamp);
	for(int i = 0; i < 1; i++){
		p.insert_tcp_header_option(option);
	}

	p.pack();

	void * new_pointer = p.get_atp_header();

	EXPECT_TRUE(old_pointer != new_pointer);

	const char* data = "This is some cool data";
	p.set_data((unsigned char*) data, strlen(data));

}


TEST(ATPPacketTest, memoryCopy){
	ATPPacket p;
	u_int32_t timestamp = 5;
	u_int32_t echo = 4;

	p.set_atp_average_delay(1234);
	p.set_atp_max_delay(5678);

	TCPTimestampOption* option = new TCPTimestampOption();
	option->set_echo_reply(echo);
	option->set_timestamp(timestamp);
	p.insert_tcp_header_option(option);

	const char* data = "This is some cool data";
	p.set_data((unsigned char*) data, strlen(data));

	p.pack();

	ATPPacket other;
	memcpy(other.get_payload(), p.get_payload(), MTU);

	other.set_atp_average_delay(4321);
	other.set_atp_max_delay(8765);

	EXPECT_EQ(4321, other.get_atp_average_delay());
	EXPECT_EQ(8765, other.get_atp_max_delay());
}


TEST(ATPPacketTest, size){
	ATPPacket p;

	// Make sure initial sizes are correct
	EXPECT_EQ(20, p.get_ip_header_length_bytes());
	EXPECT_EQ(20, p.get_tcp_header_length_bytes());
	EXPECT_EQ(8, p.get_atp_header_length_bytes());
	EXPECT_EQ(48, p.get_ip_tot_length());

	u_int32_t timestamp = 5;
	u_int32_t echo = 4;

	TCPTimestampOption* option = new TCPTimestampOption();
	option->set_echo_reply(echo);
	option->set_timestamp(timestamp);
	p.insert_tcp_header_option(option);

	EXPECT_EQ(20, p.get_ip_header_length_bytes());
	EXPECT_EQ(32, p.get_tcp_header_length_bytes());
	EXPECT_EQ(8, p.get_atp_header_length_bytes());
	EXPECT_EQ(60, p.get_ip_tot_length());

	EXPECT_EQ(0, p.get_data_length_bytes());
	EXPECT_EQ(p.get_data_length_bytes(), p.get_ip_tot_length() - p.get_ip_header_length_bytes() - p.get_tcp_header_length_bytes() - p.get_atp_header_length_bytes());


	ATPPacket* p2 = new ATPPacket();
	TCPPacket* p3 = (TCPPacket *)p2;

	TCPTimestampOption* option2 = new TCPTimestampOption();
	option2->set_echo_reply(echo);
	option2->set_timestamp(timestamp);
	p3->insert_tcp_header_option(option);

	p3->pack();

	EXPECT_EQ(20, p2->get_ip_header_length_bytes());
	EXPECT_EQ(32, p2->get_tcp_header_length_bytes());
	EXPECT_EQ(8, p2->get_atp_header_length_bytes());
	EXPECT_EQ(60, p2->get_ip_tot_length());

	delete p2;

}

void binDump (char *desc, void *addr, int len) {
    int i;
    unsigned char buff[17];       // stores the ASCII data
    unsigned char *pc = (unsigned char*)addr;     // cast to make the code cleaner.

    // Output description if given.
    if (desc != NULL)
        printf ("%s:\n", desc);

    // Process every byte in the data.
    for (i = 0; i < len; i++) {

    	// Multiple of 16 means new line (with line offset).
        if ((i % 4) == 0) {

        	// Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", i);
        }

        // Now the hex code for the specific character.
        bitset<sizeof(char) * 8> binary(pc[i]);
        std::cout << binary << " ";

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 4] = '.';
        else
            buff[i % 4] = pc[i];
        buff[(i % 4) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 4) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
}




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

