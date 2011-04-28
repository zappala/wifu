#ifndef TCPTIMESTAMPOPTIONTEST
#define TCPTIMESTAMPOPTIONTEST

#include <iostream>

#include "gtest/gtest.h"
#include "packet/TCPTimestampOption.h"
#include <iostream>

using namespace std;

namespace {

    TEST(TCPTimestampOptionTest, Constructor) {
        TCPTimestampOption ts_option;

        u_int8_t kind = 8;
        u_int8_t length = 10;

        EXPECT_EQ(kind, ts_option.get_kind());
        EXPECT_EQ(length, ts_option.get_length());
    }

    TEST(TCPTimestampOptionTest, GetSetTimestampPointer) {
        TCPTimestampOption ts_option;
        
        u_int32_t expected = 65;
        ts_option.set_timestamp(expected);

        EXPECT_EQ(expected, ts_option.get_timestamp());
        EXPECT_EQ(expected, ts_option.get_timestamp_struct_pointer()->timestamp_value_);

        ts_option.set_timestamp();
        expected = Utils::get_current_time_microseconds_32();
        EXPECT_NEAR(expected, ts_option.get_timestamp(), 100);
        EXPECT_NEAR(expected, ts_option.get_timestamp_struct_pointer()->timestamp_value_, 100);
    }

    TEST(TCPTimestampOptionTest, GetSetEchoPointer) {
        TCPTimestampOption ts_option;

        u_int32_t expected = 65;
        ts_option.set_echo_reply(expected);

        EXPECT_EQ(expected, ts_option.get_echo_reply());
        EXPECT_EQ(expected, ts_option.get_timestamp_struct_pointer()->timestamp_echo_reply_);
    }
}

#endif