#include "TCPPacketBuffer.h"

#include "packet/TCPPacket.h"
#include "gtest/gtest.h"
#include "../headers/HelperFunctions.h"
#include "../headers/RandomStringGenerator.h"
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

        EXPECT_EQ(expected, actual);
        EXPECT_EQ(data, buffer.get_continuous_data(sequence_number));
        data = "";
        EXPECT_EQ(data, buffer.get_continuous_data(sequence_number));
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
        EXPECT_EQ(expected, actual);

        actual = buffer.insert(p2);
        EXPECT_EQ(expected, actual);

        string expected_data = data;
        expected_data.append(data);
        EXPECT_EQ(expected_data, buffer.get_continuous_data(sequence_number));

        expected_data = "";
        EXPECT_EQ(expected_data, buffer.get_continuous_data(sequence_number));
        EXPECT_EQ(expected_data, buffer.get_continuous_data(sequence_number + expected));
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
        EXPECT_EQ(expected, actual);

        actual = buffer.insert(p2);
        expected = data2.size();
        EXPECT_EQ(expected, actual);

        string expected_data = data;
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num));

        expected_data = "";
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num + data.size()));

        actual = buffer.insert(p3);
        expected = 5;
        EXPECT_EQ(expected, actual);

        expected_data = "This is more data";
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num + data.size()));

        expected_data = "";
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num));
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num + data.size()));
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num + data.size() + expected_data.size()));
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
        EXPECT_EQ(expected, actual);

        actual = buffer.insert(p2);
        expected = data2.size();
        EXPECT_EQ(expected, actual);

        string expected_data = data;
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num));

        expected_data = "";
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num + data.size()));

        actual = buffer.insert(p3);
        expected = 5;
        EXPECT_EQ(expected, actual);

        expected_data = "This is more data";
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num + data.size()));

        expected_data = "";
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num));
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num + data.size()));
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num + data.size() + expected_data.size()));
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
        EXPECT_EQ(expected, actual);

        actual = buffer.insert(p2);
        expected = data2.size();
        EXPECT_EQ(expected, actual);

        string expected_data = data;
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num));

        expected_data = "";
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num + data.size()));

        actual = buffer.insert(p3);
        expected = 25;
        EXPECT_EQ(expected, actual);

        expected_data = data3;
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num + data.size()));

        expected_data = "";
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num));
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num + data.size() + expected_data.size()));
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
        EXPECT_EQ(expected, actual);

        actual = buffer.insert(p2);
        expected = data2.size();
        EXPECT_EQ(expected, actual);

        actual = buffer.insert(p3);
        expected = 25;
        EXPECT_EQ(expected, actual);

        string expected_data = data.append(data3);
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num));

        expected_data = "";
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num));
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num + expected_data.size()));
    }

    TEST(TCPPacketBufferTest, gap5) {
        TCPPacketBuffer buffer;
        //                     10        20        30        40        50
        //             123456789012345678901234567890123456789012345678901234
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
        EXPECT_EQ(expected, actual);

        actual = buffer.insert(p2);
        expected = data2.size();
        EXPECT_EQ(expected, actual);

        actual = buffer.insert(p3);
        expected = data3.size();
        EXPECT_EQ(expected, actual);

        actual = buffer.insert(p4);
        expected = data4.size() - data2.size() - data3.size();
        EXPECT_EQ(expected, actual);

        string expected_data = data.append(data4);
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num));

        expected_data = "";
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num));
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num + expected_data.size()));
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
        EXPECT_EQ(expected, actual);

        actual = buffer.insert(p2);
        expected = data2.size();
        EXPECT_EQ(expected, actual);

        actual = buffer.insert(p3);
        expected = data3.size();
        EXPECT_EQ(expected, actual);

        actual = buffer.insert(p4);
        expected = data4.size() - data2.size() - 9;
        EXPECT_EQ(expected, actual);

        string expected_data = data.append(data4).append("nted.");
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num));

        expected_data = "";
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num));
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num + expected_data.size()));
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
        EXPECT_EQ(expected, actual);

        actual = buffer.insert(p1);
        expected = 0;
        EXPECT_EQ(expected, actual);

        string expected_data = data;
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num));

        expected_data = "";
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num));
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num + expected_data.size()));
    }

    TEST(TCPPacketBufferTest, equalSequenceNumberSmallerLength) {
        TCPPacketBuffer buffer;
        //                      10        20        30        40        50
        //              123456789012345678901234567890123456789012345678901234
        string data =  "This is the data. ";
        string data2 = "This is the";

        u_int32_t data_seq_num = 1;

        TCPPacket* p1 = HelperFunctions::get_tcp_packet_with_data(data_seq_num, data);
        TCPPacket* p2 = HelperFunctions::get_tcp_packet_with_data(data_seq_num, data2);

        int actual = buffer.insert(p1);
        int expected = data.size();
        EXPECT_EQ(expected, actual);

        actual = buffer.insert(p2);
        expected = 0;
        EXPECT_EQ(expected, actual);

        string expected_data = data;
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num));

        expected_data = "";
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num));
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num + expected_data.size()));
    }

    TEST(TCPPacketBufferTest, equalSequenceNumberLargerLength) {
        TCPPacketBuffer buffer;
        //                      10        20        30        40        50
        //              123456789012345678901234567890123456789012345678901234
        string data =  "This is the";
        string data2 = "This is the data.";

        u_int32_t data_seq_num = 1;

        TCPPacket* p1 = HelperFunctions::get_tcp_packet_with_data(data_seq_num, data);
        TCPPacket* p2 = HelperFunctions::get_tcp_packet_with_data(data_seq_num, data2);

        int actual = buffer.insert(p1);
        int expected = data.size();
        EXPECT_EQ(expected, actual);

        actual = buffer.insert(p2);
        expected = data2.size() - data.size();
        EXPECT_EQ(expected, actual);

        string expected_data = data2;
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num));

        expected_data = "";
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num));
        EXPECT_EQ(expected_data, buffer.get_continuous_data(data_seq_num + expected_data.size()));
    }

    TEST(TCPPacketBufferTest, random) {
        TCPPacketBuffer buffer;

        bitset<10000> bits;
        bits.set();
        int total = bits.size();
        int max_data_length = 100;

        // start at sequence #0
        // The index in the string is equal to the sequence number
        string expected = RandomStringGenerator::get_data(total);

        //srand(4000);
        while(bits.any()) {
            int sequence_number = rand() % total;
            
            int length = rand() % (max_data_length + 1);

            if(sequence_number + length > expected.size()) {
                length = expected.size() - sequence_number;
            }
            
            if(length == 0) {
                continue;
            }
            string s = expected.substr(sequence_number, length);

            TCPPacket* p = HelperFunctions::get_tcp_packet_with_data(sequence_number, s);
            int count = buffer.insert(p);
            
            if(count < 0) {
                cout << "BAD seq num: " << p->get_tcp_sequence_number() << endl;
                cout << "BAD data length: " << p->get_data_length_bytes() << endl;
            }
            EXPECT_LE(0, count);


            for(int i = sequence_number; i < sequence_number + length; ++i) {
                bits.set(i, false);
            }
        }

        EXPECT_EQ(expected, buffer.get_continuous_data(0));
    }
}
