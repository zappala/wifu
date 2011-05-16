#include "TCPPacketBuffer.h"

#include "packet/TCPPacket.h"
#include "gtest/gtest.h"
#include "../headers/HelperFunctions.h"
using namespace std;

namespace {

    TEST(TCPPacketBufferTest, simple) {
        TCPPacketBuffer buffer;
        string data = "This is the data";
        u_int32_t sequence_number = 1;
        TCPPacket* p = HelperFunctions::get_tcp_packet_with_data(sequence_number, data);

        int actual = buffer.insert(p);
        int expected = data.size();

        EXPECT_EQ(expected, actual);
        EXPECT_EQ(data, buffer.get_continuous_data(sequence_number));
        data = "";
        EXPECT_EQ(data, buffer.get_continuous_data(sequence_number));
    }

    TEST(TCPPacketBufferTest, twoSimple) {
        TCPPacketBuffer buffer;
        string data = "This is the data";
        u_int32_t sequence_number = 1;
        u_int32_t next_seq_number = sequence_number + data.size();
        
        TCPPacket* p1 = HelperFunctions::get_tcp_packet_with_data(sequence_number, data);
        TCPPacket* p2 = HelperFunctions::get_tcp_packet_with_data(next_seq_number, data);

        int actual = buffer.insert(p1);
        int expected = data.size();
        EXPECT_EQ(expected, actual);

        actual = buffer.insert(p2);
        EXPECT_EQ(expected, actual);

        string expected_data = data;
        expected_data.append(data);
        EXPECT_EQ(expected_data, buffer.get_continuous_data(sequence_number));
        
        expected_data = "";
        EXPECT_EQ(data, buffer.get_continuous_data(sequence_number + expected));
    }
}
