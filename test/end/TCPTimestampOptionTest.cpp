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

        EXPECT_EQ(kind, *ts_option.get_kind_pointer());
        EXPECT_EQ(length, *ts_option.get_length_pointer());
    }

    TEST(TCPTimestampOptionTest, GetTimestampPointer) {
        TCPTimestampOption ts_option;
        
        u_int32_t expected = 65;
        u_int32_t* value = ts_option.get_timestamp_pointer();
        *value = expected;

        EXPECT_EQ(expected, *ts_option.get_timestamp_pointer());
        EXPECT_EQ(expected, ts_option.get_timestamp_struct_pointer()->timestamp_value_);
    }

    TEST(TCPTimestampOptionTest, GetEchoPointer) {
        TCPTimestampOption ts_option;

        u_int32_t expected = 65;
        u_int32_t* value = ts_option.get_echo_pointer();
        *value = expected;

        EXPECT_EQ(expected, *ts_option.get_echo_pointer());
        EXPECT_EQ(expected, ts_option.get_timestamp_struct_pointer()->timestamp_echo_reply_);
    }
}

#endif