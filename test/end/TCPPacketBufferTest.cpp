#include "TCPPacketBuffer.h"

#include "packet/TCPPacket.h"
#include "gtest/gtest.h"
#include "../headers/HelperFunctions.h"
#include "../headers/RandomStringGenerator.h"
#include "events/TimeoutEvent.h"
#include <bitset>

using namespace std;

namespace {

    TEST(TCPPacketBufferTest, simple) {
        TCPPacketBuffer buffer;
        string data = "This is the data";
        u_int32_t sequence_number = 1;
        TCPPacket* p = HelperFunctions::get_tcp_packet_with_data(sequence_number, data);

        int actual = buffer.insert(p);
        int expected = data.size();

        ASSERT_EQ(expected, actual);
        string b;
        buffer.get_continuous_data(sequence_number, b);
        ASSERT_EQ(data, b);

        data = "";
        b.clear();
        buffer.get_continuous_data(sequence_number, b);
        ASSERT_EQ(data, b);
    }

    TEST(TCPPacketBufferTest, twoBackToBack) {
        TCPPacketBuffer buffer;
        string data = "This is the data";
        u_int32_t sequence_number = 1;
        u_int32_t next_seq_number = sequence_number + data.size();

        TCPPacket* p1 = HelperFunctions::get_tcp_packet_with_data(sequence_number, data);
        TCPPacket* p2 = HelperFunctions::get_tcp_packet_with_data(next_seq_number, data);

        int actual = buffer.insert(p1);
        int expected = data.size();
        ASSERT_EQ(expected, actual);

        actual = buffer.insert(p2);
        ASSERT_EQ(expected, actual);

        string expected_data = data;
        expected_data.append(data);
        string b;
        buffer.get_continuous_data(sequence_number, b);
        ASSERT_EQ(expected_data, b);

        expected_data = "";
        b.clear();
        buffer.get_continuous_data(sequence_number, b);
        ASSERT_EQ(expected_data, b);

        b.clear();
        buffer.get_continuous_data(sequence_number + expected, b);
        ASSERT_EQ(expected_data, b);
    }

    TEST(TCPPacketBufferTest, gap1) {
        TCPPacketBuffer buffer;
        //                     10        20        30
        //             123456789012345678901234567890123456
        string data = "This is the data. ";
        string data2 = "is more data";
        string data3 = "This is more d";

        u_int32_t data_seq_num = 1;
        u_int32_t data2_seq_num = 24;
        u_int32_t data3_seq_num = 19;

        TCPPacket* p1 = HelperFunctions::get_tcp_packet_with_data(data_seq_num, data);
        TCPPacket* p2 = HelperFunctions::get_tcp_packet_with_data(data2_seq_num, data2);
        TCPPacket* p3 = HelperFunctions::get_tcp_packet_with_data(data3_seq_num, data3);

        int actual = buffer.insert(p1);
        int expected = data.size();
        ASSERT_EQ(expected, actual);

        actual = buffer.insert(p2);
        expected = data2.size();
        ASSERT_EQ(expected, actual);

        string expected_data = data;
        string b;
        buffer.get_continuous_data(data_seq_num, b);
        ASSERT_EQ(expected_data, b);


        expected_data = "";
        b.clear();
        buffer.get_continuous_data(data_seq_num + data.size(), b);
        ASSERT_EQ(expected_data, b);

        actual = buffer.insert(p3);
        expected = 5;
        ASSERT_EQ(expected, actual);

        expected_data = "This is more data";
        b.clear();
        buffer.get_continuous_data(data_seq_num + data.size(), b);
        ASSERT_EQ(expected_data, b);

        expected_data = "";
        b.clear();
        buffer.get_continuous_data(data_seq_num, b);
        ASSERT_EQ(expected_data, b);
        b.clear();
        buffer.get_continuous_data(data_seq_num + data.size(), b);
        ASSERT_EQ(expected_data, b);
        b.clear();
        buffer.get_continuous_data(data_seq_num + data.size() + expected_data.size(), b);
        ASSERT_EQ(expected_data, b);
    }

    TEST(TCPPacketBufferTest, gap2) {
        TCPPacketBuffer buffer;
        //                     10        20        30
        //             123456789012345678901234567890123456
        string data = "This is the data. ";
        string data2 = "is more data";
        string data3 = "This ";

        u_int32_t data_seq_num = 1;
        u_int32_t data2_seq_num = 24;
        u_int32_t data3_seq_num = 19;

        TCPPacket* p1 = HelperFunctions::get_tcp_packet_with_data(data_seq_num, data);
        TCPPacket* p2 = HelperFunctions::get_tcp_packet_with_data(data2_seq_num, data2);
        TCPPacket* p3 = HelperFunctions::get_tcp_packet_with_data(data3_seq_num, data3);

        int actual = buffer.insert(p1);
        int expected = data.size();
        ASSERT_EQ(expected, actual);

        actual = buffer.insert(p2);
        expected = data2.size();
        ASSERT_EQ(expected, actual);

        string expected_data = data;
        string b;
        buffer.get_continuous_data(data_seq_num, b);
        ASSERT_EQ(expected_data, b);

        expected_data = "";
        b.clear();
        buffer.get_continuous_data(data_seq_num + data.size(), b);
        ASSERT_EQ(expected_data, b);

        actual = buffer.insert(p3);
        expected = 5;
        ASSERT_EQ(expected, actual);

        expected_data = "This is more data";
        b.clear();
        buffer.get_continuous_data(data_seq_num + data.size(), b);
        ASSERT_EQ(expected_data, b);

        expected_data = "";
        b.clear();
        buffer.get_continuous_data(data_seq_num, b);
        ASSERT_EQ(expected_data, b);
        b.clear();
        buffer.get_continuous_data(data_seq_num + data.size(), b);
        ASSERT_EQ(expected_data, b);
        b.clear();
        buffer.get_continuous_data(data_seq_num + data.size() + expected_data.size(), b);
        ASSERT_EQ(expected_data, b);
    }

    TEST(TCPPacketBufferTest, gap3) {
        TCPPacketBuffer buffer;
        //                     10        20        30        40        50
        //             123456789012345678901234567890123456789012345678901234
        string data = "This is the data. ";
        string data2 = "is more data";
        string data3 = "This is more data than I ever wanted.";

        u_int32_t data_seq_num = 1;
        u_int32_t data2_seq_num = 24;
        u_int32_t data3_seq_num = 19;

        TCPPacket* p1 = HelperFunctions::get_tcp_packet_with_data(data_seq_num, data);
        TCPPacket* p2 = HelperFunctions::get_tcp_packet_with_data(data2_seq_num, data2);
        TCPPacket* p3 = HelperFunctions::get_tcp_packet_with_data(data3_seq_num, data3);

        int actual = buffer.insert(p1);
        int expected = data.size();
        ASSERT_EQ(expected, actual);

        actual = buffer.insert(p2);
        expected = data2.size();
        ASSERT_EQ(expected, actual);

        string expected_data = data;
        string b;
        buffer.get_continuous_data(data_seq_num, b);
        ASSERT_EQ(expected_data, b);

        expected_data = "";
        b.clear();
        buffer.get_continuous_data(data_seq_num + data.size(), b);
        ASSERT_EQ(expected_data, b);

        actual = buffer.insert(p3);
        expected = 25;
        ASSERT_EQ(expected, actual);

        expected_data = data3;
        b.clear();
        buffer.get_continuous_data(data_seq_num + data.size(), b);
        ASSERT_EQ(expected_data, b);

        expected_data = "";
        b.clear();
        buffer.get_continuous_data(data_seq_num, b);
        ASSERT_EQ(expected_data, b);
        b.clear();
        buffer.get_continuous_data(data_seq_num + data.size() + expected_data.size(), b);
        ASSERT_EQ(expected_data, b);
    }

    TEST(TCPPacketBufferTest, gap4) {
        TCPPacketBuffer buffer;
        //                     10        20        30        40        50
        //             123456789012345678901234567890123456789012345678901234
        string data = "This is the data. ";
        string data2 = "is more data";
        string data3 = "This is more data than I ever wanted.";

        u_int32_t data_seq_num = 1;
        u_int32_t data2_seq_num = 24;
        u_int32_t data3_seq_num = 19;

        TCPPacket* p1 = HelperFunctions::get_tcp_packet_with_data(data_seq_num, data);
        TCPPacket* p2 = HelperFunctions::get_tcp_packet_with_data(data2_seq_num, data2);
        TCPPacket* p3 = HelperFunctions::get_tcp_packet_with_data(data3_seq_num, data3);

        int actual = buffer.insert(p1);
        int expected = data.size();
        ASSERT_EQ(expected, actual);

        actual = buffer.insert(p2);
        expected = data2.size();
        ASSERT_EQ(expected, actual);

        actual = buffer.insert(p3);
        expected = 25;
        ASSERT_EQ(expected, actual);

        string expected_data = data.append(data3);
        string b;
        buffer.get_continuous_data(data_seq_num, b);
        ASSERT_EQ(expected_data, b);

        expected_data = "";
        b.clear();
        buffer.get_continuous_data(data_seq_num, b);
        ASSERT_EQ(expected_data, b);
        b.clear();
        buffer.get_continuous_data(data_seq_num + expected_data.size(), b);
        ASSERT_EQ(expected_data, b);
    }

    TEST(TCPPacketBufferTest, gap5) {
        TCPPacketBuffer buffer;
        //                     10        20        30        40        50
        //             1234567890123456789012345678901234567890123456789012345
        string data = "This is the data. ";
        string data2 = "is more data";
        string data3 = "I ever";
        string data4 = "This is more data than I ever wanted.";

        u_int32_t data_seq_num = 1;
        u_int32_t data2_seq_num = 24;
        u_int32_t data3_seq_num = 42;
        u_int32_t data4_seq_num = 19;

        TCPPacket* p1 = HelperFunctions::get_tcp_packet_with_data(data_seq_num, data);
        TCPPacket* p2 = HelperFunctions::get_tcp_packet_with_data(data2_seq_num, data2);
        TCPPacket* p3 = HelperFunctions::get_tcp_packet_with_data(data3_seq_num, data3);
        TCPPacket* p4 = HelperFunctions::get_tcp_packet_with_data(data4_seq_num, data4);

        int actual = buffer.insert(p1);
        int expected = data.size();
        ASSERT_EQ(expected, actual);

        actual = buffer.insert(p2);
        expected = data2.size();
        ASSERT_EQ(expected, actual);

        actual = buffer.insert(p3);
        expected = data3.size();
        ASSERT_EQ(expected, actual);

        actual = buffer.insert(p4);
        expected = data4.size() - data2.size() - data3.size();
        ASSERT_EQ(expected, actual);

        string expected_data = data.append(data4);
        string b;
        buffer.get_continuous_data(data_seq_num, b);
        ASSERT_EQ(expected_data, b);

        expected_data = "";
        b.clear();
        buffer.get_continuous_data(data_seq_num, b);
        ASSERT_EQ(expected_data, b);
        b.clear();
        buffer.get_continuous_data(data_seq_num + expected_data.size(), b);
        ASSERT_EQ(expected_data, b);
    }

    TEST(TCPPacketBufferTest, gap6) {
        TCPPacketBuffer buffer;
        //                     10        20        30        40        50
        //             123456789012345678901234567890123456789012345678901234
        string data = "This is the data. ";
        string data2 = "is more data";
        string data3 = "I ever wanted.";
        string data4 = "This is more data than I ever wa";

        u_int32_t data_seq_num = 1;
        u_int32_t data2_seq_num = 24;
        u_int32_t data3_seq_num = 42;
        u_int32_t data4_seq_num = 19;

        TCPPacket* p1 = HelperFunctions::get_tcp_packet_with_data(data_seq_num, data);
        TCPPacket* p2 = HelperFunctions::get_tcp_packet_with_data(data2_seq_num, data2);
        TCPPacket* p3 = HelperFunctions::get_tcp_packet_with_data(data3_seq_num, data3);
        TCPPacket* p4 = HelperFunctions::get_tcp_packet_with_data(data4_seq_num, data4);

        int actual = buffer.insert(p1);
        int expected = data.size();
        ASSERT_EQ(expected, actual);

        actual = buffer.insert(p2);
        expected = data2.size();
        ASSERT_EQ(expected, actual);

        actual = buffer.insert(p3);
        expected = data3.size();
        ASSERT_EQ(expected, actual);

        actual = buffer.insert(p4);
        expected = data4.size() - data2.size() - 9;
        ASSERT_EQ(expected, actual);

        string expected_data = data.append(data4).append("nted.");
        string b;
        buffer.get_continuous_data(data_seq_num, b);
        ASSERT_EQ(expected_data, b);

        expected_data = "";
        b.clear();
        buffer.get_continuous_data(data_seq_num, b);
        ASSERT_EQ(expected_data, b);
        b.clear();
        buffer.get_continuous_data(data_seq_num + expected_data.size(), b);
        ASSERT_EQ(expected_data, b);
    }

    TEST(TCPPacketBufferTest, equalSequenceNumberEqualLength) {
        TCPPacketBuffer buffer;
        //                     10        20        30        40        50
        //             123456789012345678901234567890123456789012345678901234
        string data = "This is the data. ";

        u_int32_t data_seq_num = 1;

        TCPPacket* p1 = HelperFunctions::get_tcp_packet_with_data(data_seq_num, data);

        int actual = buffer.insert(p1);
        int expected = data.size();
        ASSERT_EQ(expected, actual);

        actual = buffer.insert(p1);
        expected = 0;
        ASSERT_EQ(expected, actual);

        string expected_data = data;
        string b;
        buffer.get_continuous_data(data_seq_num, b);
        ASSERT_EQ(expected_data, b);

        expected_data = "";
        b.clear();
        buffer.get_continuous_data(data_seq_num, b);
        ASSERT_EQ(expected_data, b);
        b.clear();
        buffer.get_continuous_data(data_seq_num + expected_data.size(), b);
        ASSERT_EQ(expected_data, b);
    }

    TEST(TCPPacketBufferTest, equalSequenceNumberSmallerLength) {
        TCPPacketBuffer buffer;
        //                      10        20        30        40        50
        //              123456789012345678901234567890123456789012345678901234
        string data = "This is the data. ";
        string data2 = "This is the";

        u_int32_t data_seq_num = 1;

        TCPPacket* p1 = HelperFunctions::get_tcp_packet_with_data(data_seq_num, data);
        TCPPacket* p2 = HelperFunctions::get_tcp_packet_with_data(data_seq_num, data2);

        int actual = buffer.insert(p1);
        int expected = data.size();
        ASSERT_EQ(expected, actual);

        actual = buffer.insert(p2);
        expected = 0;
        ASSERT_EQ(expected, actual);

        string expected_data = data;
        string b;
        buffer.get_continuous_data(data_seq_num, b);
        ASSERT_EQ(expected_data, b);

        expected_data = "";
        b.clear();
        buffer.get_continuous_data(data_seq_num, b);
        ASSERT_EQ(expected_data, b);
        b.clear();
        buffer.get_continuous_data(data_seq_num + expected_data.size(), b);
        ASSERT_EQ(expected_data, b);
    }

    TEST(TCPPacketBufferTest, equalSequenceNumberLargerLength) {
        TCPPacketBuffer buffer;
        //                      10        20        30        40        50
        //              123456789012345678901234567890123456789012345678901234
        string data = "This is the";
        string data2 = "This is the data.";

        u_int32_t data_seq_num = 1;

        TCPPacket* p1 = HelperFunctions::get_tcp_packet_with_data(data_seq_num, data);
        TCPPacket* p2 = HelperFunctions::get_tcp_packet_with_data(data_seq_num, data2);

        int actual = buffer.insert(p1);
        int expected = data.size();
        ASSERT_EQ(expected, actual);

        actual = buffer.insert(p2);
        expected = data2.size() - data.size();
        ASSERT_EQ(expected, actual);

        string expected_data = data2;
        string b;
        buffer.get_continuous_data(data_seq_num, b);
        ASSERT_EQ(expected_data, b);

        expected_data = "";
        b.clear();
        buffer.get_continuous_data(data_seq_num, b);
        ASSERT_EQ(expected_data, b);
        b.clear();
        buffer.get_continuous_data(data_seq_num + expected_data.size(), b);
        ASSERT_EQ(expected_data, b);
    }

    TEST(TCPPacketBufferTest, random) {

        TCPPacketBuffer buffer;

        bitset < USHRT_MAX > bits;
        bits.set();
        int total = bits.size();
        int max_data_length = 1400;


        // start at sequence #0
        // The index in the string is equal to the sequence number
        string expected = RandomStringGenerator::get_data(total);
        int total_inserted = 0;


        while (bits.any()) {
            int sequence_number = rand() % total;

            int length = rand() % (max_data_length + 1);

            if (sequence_number + length > expected.size()) {
                length = expected.size() - sequence_number;
            }

            if (length == 0) {
                continue;
            }
            string s = expected.substr(sequence_number, length);

            TCPPacket* p = HelperFunctions::get_tcp_packet_with_data(sequence_number, s);
            int count = buffer.insert(p);
            ASSERT_LE(0, count);
            total_inserted += count;

            for (int i = sequence_number; i < sequence_number + length; ++i) {
                bits.set(i, false);
            }
        }

        string b;
        buffer.get_continuous_data(0, b);
        ASSERT_EQ(expected, b);
        ASSERT_EQ(expected.length(), total_inserted);
    }
}
