#include "TCPPacketBuffer.h"

#include "packet/TCPPacket.h"
#include "gtest/gtest.h"
#include "../headers/HelperFunctions.h"
#include "../headers/RandomStringGenerator.h"
#include "events/framework_events/TimeoutEvent.h"
#include <bitset>

using namespace std;

namespace {

    TEST(TCPPacketBufferTest, simple) {
        TCPPacketBuffer buffer;
        gcstring data = "This is the data";
        u_int32_t sequence_number = 1;
        buffer.set_first_sequence_number(sequence_number);
        TCPPacket* p = HelperFunctions::get_tcp_packet_with_data(sequence_number, data);

        int actual = buffer.insert(p);
        int expected = data.size();

        ASSERT_EQ(expected, actual);
        gcstring b;
        buffer.get_continuous_data(sequence_number, b);
        ASSERT_EQ(data, b);

        data = "";
        b.clear();
        buffer.get_continuous_data(sequence_number, b);
        ASSERT_EQ(data, b);
    }

    TEST(TCPPacketBufferTest, twoBackToBack) {
        TCPPacketBuffer buffer;
        gcstring data = "This is the data";
        u_int32_t sequence_number = 1;
        buffer.set_first_sequence_number(sequence_number);
        u_int32_t next_seq_number = sequence_number + data.size();

        TCPPacket* p1 = HelperFunctions::get_tcp_packet_with_data(sequence_number, data);
        TCPPacket* p2 = HelperFunctions::get_tcp_packet_with_data(next_seq_number, data);

        int actual = buffer.insert(p1);
        int expected = data.size();
        ASSERT_EQ(expected, actual);

        actual = buffer.insert(p2);
        ASSERT_EQ(expected, actual);

        gcstring expected_data = data;
        expected_data.append(data);
        gcstring b;
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
        gcstring data = "This is the data. ";
        gcstring data2 = "is more data";
        gcstring data3 = "This is more d";

        u_int32_t data_seq_num = 1;
        buffer.set_first_sequence_number(data_seq_num);
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

        gcstring expected_data = data;
        gcstring b;
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
        gcstring data = "This is the data. ";
        gcstring data2 = "is more data";
        gcstring data3 = "This ";

        u_int32_t data_seq_num = 1;
        buffer.set_first_sequence_number(data_seq_num);
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

        gcstring expected_data = data;
        gcstring b;
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
        gcstring data = "This is the data. ";
        gcstring data2 = "is more data";
        gcstring data3 = "This is more data than I ever wanted.";

        u_int32_t data_seq_num = 1;
        buffer.set_first_sequence_number(data_seq_num);
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

        gcstring expected_data = data;
        gcstring b;
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
        gcstring data = "This is the data. ";
        gcstring data2 = "is more data";
        gcstring data3 = "This is more data than I ever wanted.";

        u_int32_t data_seq_num = 1;
        buffer.set_first_sequence_number(data_seq_num);
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

        gcstring expected_data = data.append(data3);
        gcstring b;
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
        gcstring data = "This is the data. ";
        gcstring data2 = "is more data";
        gcstring data3 = "I ever";
        gcstring data4 = "This is more data than I ever wanted.";

        u_int32_t data_seq_num = 1;
        buffer.set_first_sequence_number(data_seq_num);
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

        gcstring expected_data = data.append(data4);
        gcstring b;
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
        gcstring data = "This is the data. ";
        gcstring data2 = "is more data";
        gcstring data3 = "I ever wanted.";
        gcstring data4 = "This is more data than I ever wa";

        u_int32_t data_seq_num = 1;
        buffer.set_first_sequence_number(data_seq_num);
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

        gcstring expected_data = data.append(data4).append("nted.");
        gcstring b;
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

    TEST(TCPPacketBufferTest, lessThanAlredyRetrieved) {
        TCPPacketBuffer buffer;
        //                     10        20        30        40        50
        //             12345678901234567890123456789012345678901234567890123456
        gcstring data = "This is the data. ";
        gcstring data2 = "is more data";
        gcstring data3 = "I ever wanted.";
        gcstring data4 = "is the data. This is more data than I ever wa";

        u_int32_t data_seq_num = 1;
        buffer.set_first_sequence_number(data_seq_num);
        u_int32_t data2_seq_num = 24;
        u_int32_t data3_seq_num = 42;
        u_int32_t data4_seq_num = 6;

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

        gcstring expected_data = data;
        gcstring b;
        buffer.get_continuous_data(data_seq_num, b);
        ASSERT_EQ(expected_data, b);

        actual = buffer.insert(p4);
        expected = 11;
        ASSERT_EQ(expected, actual);

        data_seq_num = 19;
        expected_data = "This is more data than I ever wanted.";
        b.clear();
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
        gcstring data = "This is the data. ";

        u_int32_t data_seq_num = 1;
        buffer.set_first_sequence_number(data_seq_num);

        TCPPacket* p1 = HelperFunctions::get_tcp_packet_with_data(data_seq_num, data);

        int actual = buffer.insert(p1);
        int expected = data.size();
        ASSERT_EQ(expected, actual);

        actual = buffer.insert(p1);
        expected = 0;
        ASSERT_EQ(expected, actual);

        gcstring expected_data = data;
        gcstring b;
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
        gcstring data = "This is the data. ";
        gcstring data2 = "This is the";

        u_int32_t data_seq_num = 1;
        buffer.set_first_sequence_number(data_seq_num);

        TCPPacket* p1 = HelperFunctions::get_tcp_packet_with_data(data_seq_num, data);
        TCPPacket* p2 = HelperFunctions::get_tcp_packet_with_data(data_seq_num, data2);

        int actual = buffer.insert(p1);
        int expected = data.size();
        ASSERT_EQ(expected, actual);

        actual = buffer.insert(p2);
        expected = 0;
        ASSERT_EQ(expected, actual);

        gcstring expected_data = data;
        gcstring b;
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
        gcstring data = "This is the";
        gcstring data2 = "This is the data.";

        u_int32_t data_seq_num = 1;
        buffer.set_first_sequence_number(data_seq_num);

        TCPPacket* p1 = HelperFunctions::get_tcp_packet_with_data(data_seq_num, data);
        TCPPacket* p2 = HelperFunctions::get_tcp_packet_with_data(data_seq_num, data2);

        int actual = buffer.insert(p1);
        int expected = data.size();
        ASSERT_EQ(expected, actual);

        actual = buffer.insert(p2);
        expected = data2.size() - data.size();
        ASSERT_EQ(expected, actual);

        gcstring expected_data = data2;
        gcstring b;
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
        buffer.set_first_sequence_number(0);

        bitset < 2000 > bits;
        bits.set();
        int total = bits.size();
        int max_data_length = 2;


        // start at sequence #0
        // The index in the string is equal to the sequence number
        gcstring expected = RandomStringGenerator::get_data(total);
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
            gcstring s = expected.substr(sequence_number, length);

            TCPPacket* p = HelperFunctions::get_tcp_packet_with_data(sequence_number, s);
            int count = buffer.insert(p);
            ASSERT_LE(0, count);
            total_inserted += count;

            for (int i = sequence_number; i < sequence_number + length; ++i) {
                bits.set(i, false);
            }
        }

        gcstring b;
        buffer.get_continuous_data(0, b);
        ASSERT_EQ(expected, b);
        ASSERT_EQ(expected.length(), total_inserted);
    }

    TEST(TCPPacketBufferTest, randomWithIntermitentRemoval) {

        TCPPacketBuffer buffer;
        buffer.set_first_sequence_number(0);
        bitset < 20000 > bits;
        bits.set();
        int total = bits.size();
        int max_data_length = 1400;


        // start at sequence #0
        // The index in the string is equal to the sequence number
        gcstring expected = RandomStringGenerator::get_data(total);
        int total_inserted = 0;
        int total_removed = 0;

        int nxt = 0;
        gcstring actual = "";

        while (bits.any()) {
            int sequence_number = (rand() % total) + nxt;

            if (sequence_number > expected.size()) {
                continue;
            }

            int length = rand() % (max_data_length + 1);

            if (sequence_number + length > expected.size()) {
                length = expected.size() - sequence_number;
            }

            if (length == 0) {
                continue;
            }

            gcstring s = expected.substr(sequence_number, length);

            TCPPacket* p = HelperFunctions::get_tcp_packet_with_data(sequence_number, s);
            int count = buffer.insert(p);
            ASSERT_LE(0, count);
            total_inserted += count;

            for (int i = sequence_number; i < sequence_number + length; ++i) {
                bits.set(i, false);
            }

            // remove data
            int before_size = actual.size();
            buffer.get_continuous_data(nxt, actual);
            int removed = actual.size() - before_size;

            before_size = actual.size();
            buffer.get_continuous_data(nxt, actual);
            ASSERT_EQ(0, actual.size() - before_size);

            total_removed += removed;
            nxt += removed;
        }

        ASSERT_EQ(expected, actual);
        ASSERT_EQ(expected.length(), total_inserted);
        ASSERT_EQ(expected.length(), total_removed);
    }

    TEST(TCPPacketBufferTest, dropOne) {

        TCPPacketBuffer buffer;
        buffer.set_first_sequence_number(0);
        int num_bytes = 500;
        int difference = 5;

        // start at sequence #0
        // The index in the string is equal to the sequence number
        gcstring expected = RandomStringGenerator::get_data(num_bytes);
        //        cout << "Expected: " << expected << endl;
        int total_inserted = 0;
        int total_removed = 0;
        gcstring actual = "";

        // Insert first packet
        int data_seq_num = 0;
        gcstring data = expected.substr(data_seq_num, 1);
        TCPPacket* p0 = HelperFunctions::get_tcp_packet_with_data(data_seq_num, data);
        total_inserted += buffer.insert(p0);
        //        cout << "Inserting: " << data << endl;

        // Remove first packet
        int before_size = actual.size();
        buffer.get_continuous_data(data_seq_num, actual);
        total_removed += actual.size() - before_size;

        ASSERT_EQ(data, actual);
        ASSERT_EQ(total_inserted, total_removed);

        data_seq_num++;

        for (int i = 0; i < 100; i++) {
            before_size = actual.size();
            buffer.get_continuous_data(data_seq_num, actual);
            total_removed += actual.size() - before_size;
            ASSERT_EQ(data, actual);
            ASSERT_EQ(total_inserted, total_removed);
        }

        for (data_seq_num = 2; data_seq_num < num_bytes - difference; data_seq_num++) {
            int length = 1;
            data = expected.substr(data_seq_num, length);
            //            cout << "Inserting: " << data << endl;
            TCPPacket* p = HelperFunctions::get_tcp_packet_with_data(data_seq_num, data);
            total_inserted += buffer.insert(p);
        }

        // insert sequence number 1
        data_seq_num = 1;
        data = expected.substr(data_seq_num, num_bytes - difference - data_seq_num - 1);
        //        cout << "Inserting: " << data << endl;
        TCPPacket* p1 = HelperFunctions::get_tcp_packet_with_data(data_seq_num, data);
        total_inserted += buffer.insert(p1);

        // insert the remaining bytes
        for (data_seq_num = num_bytes - difference - data_seq_num; data_seq_num < num_bytes; data_seq_num++) {
            int length = (rand() % 2) + 1;
            if (data_seq_num + 1 == num_bytes) {
                length = 1;
            }
            data = expected.substr(data_seq_num, 1);
            //            cout << "Inserting: " << data << endl;
            TCPPacket* p = HelperFunctions::get_tcp_packet_with_data(data_seq_num, data);
            total_inserted += buffer.insert(p);
        }

        data_seq_num = 1;

        // remove all packets
        before_size = actual.size();
        buffer.get_continuous_data(data_seq_num, actual);
        int after_size = actual.size();

        ASSERT_EQ(num_bytes - 1, after_size - before_size);

        total_removed += after_size - before_size;

        ASSERT_EQ(expected, actual);
        ASSERT_EQ(expected.length(), total_inserted);
        ASSERT_EQ(expected.length(), total_removed);
    }

    TEST(TCPPacketBufferTest, remove) {
        TCPPacketBuffer buffer;
        gcstring data = "This is the data";
        u_int32_t sequence_number = 1;
        buffer.set_first_sequence_number(sequence_number);
        TCPPacket* p = HelperFunctions::get_tcp_packet_with_data(sequence_number, data);

        int actual = buffer.insert(p);
        int expected = data.size();
        ASSERT_EQ(expected, actual);

        TCPPacket* erased = buffer.remove(p);
        ASSERT_EQ(p, erased);

        actual = buffer.size();
        expected = 0;
        ASSERT_EQ(expected, actual);
    }


}
