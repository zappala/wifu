#ifndef _TCPHEADEROPTIONLENGTHVISIORTEST
#define _TCPHEADEROPTIONLENGTHVISIORTEST

#include "gtest/gtest.h"
#include "visitors/GetTCPHeaderOptionsLengthVisitor.h"
#include "packet/TCPHeaderOption.h"

using namespace std;

namespace {

    TEST(GetTCPHeaderOptionsLengthVisitorTest, TestZeroBytes) {
        GetTCPHeaderOptionsLengthVisitor visitor;

        u_int8_t expected_words = 0;
        u_int8_t actual_words = visitor.get_padded_length();
        EXPECT_EQ(expected_words, actual_words);
    }

    TEST(GetTCPHeaderOptionsLengthVisitorTest, TestOneByte) {
        TCPHeaderOption option(1, 1);
        GetTCPHeaderOptionsLengthVisitor visitor;
        visitor.visit(&option);

        u_int8_t expected_words = 1;
        u_int8_t actual_words = visitor.get_padded_length();
        EXPECT_EQ(expected_words, actual_words);
    }

    TEST(GetTCPHeaderOptionsLengthVisitorTest, TestTwoBytes) {
        TCPHeaderOption option(1, 2);
        GetTCPHeaderOptionsLengthVisitor visitor;
        visitor.visit(&option);

        u_int8_t expected_words = 1;
        u_int8_t actual_words = visitor.get_padded_length();
        EXPECT_EQ(expected_words, actual_words);
    }

    TEST(GetTCPHeaderOptionsLengthVisitorTest, TestThreeBytes) {
        TCPHeaderOption option(1, 3);
        GetTCPHeaderOptionsLengthVisitor visitor;
        visitor.visit(&option);

        u_int8_t expected_words = 1;
        u_int8_t actual_words = visitor.get_padded_length();
        EXPECT_EQ(expected_words, actual_words);
    }

    TEST(GetTCPHeaderOptionsLengthVisitorTest, TestFourBytes) {
        TCPHeaderOption option(1, 4);
        GetTCPHeaderOptionsLengthVisitor visitor;
        visitor.visit(&option);

        u_int8_t expected_words = 1;
        u_int8_t actual_words = visitor.get_padded_length();
        EXPECT_EQ(expected_words, actual_words);
    }

    TEST(GetTCPHeaderOptionsLengthVisitorTest, TestFiveBytes) {
        TCPHeaderOption option(1, 5);
        GetTCPHeaderOptionsLengthVisitor visitor;
        visitor.visit(&option);

        u_int8_t expected_words = 2;
        u_int8_t actual_words = visitor.get_padded_length();
        EXPECT_EQ(expected_words, actual_words);
    }

    TEST(GetTCPHeaderOptionsLengthVisitorTest, TestManyBytes) {
        for (int i = 0; i <= 40; i++) {
            TCPHeaderOption option(1, i);
            GetTCPHeaderOptionsLengthVisitor visitor;
            visitor.visit(&option);

            u_int8_t expected_words = i / 4;
            if (i % 4 != 0) {
                expected_words += 1;
            }

            u_int8_t actual_words = visitor.get_padded_length();
            EXPECT_EQ(expected_words, actual_words);
        }
    }

    TEST(GetTCPHeaderOptionsLengthVisitorTest, TestManyOptionsBytes) {
        TCPHeaderOption option1(1, 1);
        TCPHeaderOption option2(1, 2);
        TCPHeaderOption option3(1, 3);

        GetTCPHeaderOptionsLengthVisitor visitor;
        visitor.visit(&option1);
        visitor.visit(&option2);
        visitor.visit(&option3);

        u_int8_t expected_words = 2;
        u_int8_t actual_words = visitor.get_padded_length();
        EXPECT_EQ(expected_words, actual_words);
    }

    TEST(GetTCPHeaderOptionsLengthVisitorTest, TestManyOptionsBytes2) {
        TCPHeaderOption option1(1, 10);
        TCPHeaderOption option2(1, 4);

        GetTCPHeaderOptionsLengthVisitor visitor;
        visitor.visit(&option1);
        visitor.visit(&option2);

        u_int8_t expected_words = 4;
        u_int8_t actual_words = visitor.get_padded_length();
        EXPECT_EQ(expected_words, actual_words);
    }

}

#endif /*_TCPHEADEROPTIONLENGTHVISIORTEST*/

