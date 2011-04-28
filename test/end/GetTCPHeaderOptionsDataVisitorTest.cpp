#ifndef _TCPHEADEROPTIONDATAVISIORTEST
#define _TCPHEADEROPTIONDATAVISIORTEST

#include "gtest/gtest.h"
#include "visitors/GetTCPHeaderOptionsDataVisitor.h"
#include "packet/TCPHeaderOption.h"
#include "packet/TCPTimestampOption.h"

using namespace std;

namespace {

    TEST(GetTCPHeaderOptionsDataVisitorTest, TestTCPTimestampOption) {

    }

    TEST(GetTCPHeaderOptionsDataVisitorTest, TestTwoBytes) {
        u_int8_t kind = 1;
        u_int8_t length = 2;
        TCPHeaderOption option(kind, length);

        unsigned char buffer[TCP_HEADER_OPTION_MAX_SIZE];
        memset(buffer, 'x', TCP_HEADER_OPTION_MAX_SIZE);
        GetTCPHeaderOptionsDataVisitor visitor(buffer);
        visitor.visit(&option);
        visitor.append_padding();

        u_int8_t expected_words = 1;
        u_int8_t expected_padding = 2;

        unsigned char expected_buffer[TCP_HEADER_OPTION_MAX_SIZE];
        memset(expected_buffer, 'x', TCP_HEADER_OPTION_MAX_SIZE);
        memcpy(expected_buffer, &kind, 1);
        memcpy(expected_buffer + 1, &length, 1);
        memset(expected_buffer + 2, 0, expected_padding);

        EXPECT_EQ(expected_words, visitor.get_padded_length());
        EXPECT_EQ(expected_padding, visitor.get_num_padded_bytes());
        EXPECT_TRUE(!memcmp(expected_buffer, buffer, TCP_HEADER_OPTION_MAX_SIZE));
    }

    TEST(GetTCPHeaderOptionsDataVisitorTest, TestThreeBytes) {
        u_int8_t kind = 1;
        u_int8_t length = 3;
        const char* value = "a";
        u_int8_t value_length = strlen(value);

        unsigned char buffer[TCP_HEADER_OPTION_MAX_SIZE];
        memset(buffer, 'x', TCP_HEADER_OPTION_MAX_SIZE);

        TCPHeaderOption option(kind, length);
        option.set_value((unsigned char*) value, value_length);

        GetTCPHeaderOptionsDataVisitor visitor(buffer);
        visitor.visit(&option);
        visitor.append_padding();

        u_int8_t expected_words = 1;
        u_int8_t expected_padding = 1;

        unsigned char expected_buffer[TCP_HEADER_OPTION_MAX_SIZE];
        memset(expected_buffer, 'x', TCP_HEADER_OPTION_MAX_SIZE);
        memcpy(expected_buffer, &kind, 1);
        memcpy(expected_buffer + 1, &length, 1);
        memcpy(expected_buffer + 2, value, value_length);
        memset(expected_buffer + 2 + value_length, 0, expected_padding);

        EXPECT_EQ(expected_words, visitor.get_padded_length());
        EXPECT_EQ(expected_padding, visitor.get_num_padded_bytes());
        EXPECT_TRUE(!memcmp(expected_buffer, buffer, TCP_HEADER_OPTION_MAX_SIZE));

    }

    TEST(GetTCPHeaderOptionsDataVisitorTest, TestFourBytes) {
        u_int8_t kind = 1;
        u_int8_t length = 4;
        const char* value = "ab";
        u_int8_t value_length = strlen(value);

        unsigned char buffer[TCP_HEADER_OPTION_MAX_SIZE];
        memset(buffer, 'x', TCP_HEADER_OPTION_MAX_SIZE);

        TCPHeaderOption option(kind, length);
        option.set_value((unsigned char*) value, value_length);

        GetTCPHeaderOptionsDataVisitor visitor(buffer);
        visitor.visit(&option);
        visitor.append_padding();

        u_int8_t expected_words = 1;
        u_int8_t expected_padding = 0;

        unsigned char expected_buffer[TCP_HEADER_OPTION_MAX_SIZE];
        memset(expected_buffer, 'x', TCP_HEADER_OPTION_MAX_SIZE);
        memcpy(expected_buffer, &kind, 1);
        memcpy(expected_buffer + 1, &length, 1);
        memcpy(expected_buffer + 2, value, value_length);
        memset(expected_buffer + 2 + value_length, 0, expected_padding);

        EXPECT_EQ(expected_words, visitor.get_padded_length());
        EXPECT_EQ(expected_padding, visitor.get_num_padded_bytes());
        EXPECT_TRUE(!memcmp(expected_buffer, buffer, TCP_HEADER_OPTION_MAX_SIZE));

    }

}

#endif /*_TCPHEADEROPTIONDATAVISIORTEST*/


