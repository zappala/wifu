#ifndef TCPTHEADEROPTIONTEST
#define TCPTHEADEROPTIONTEST

#include <iostream>

#include "gtest/gtest.h"
#include "packet/TCPHeaderOption.h"
#include <iostream>

using namespace std;

namespace {

    TEST(TCPHeaderOptionTest, Constructor) {
        u_int8_t kind = 1;
        u_int8_t length = 2;

        TCPHeaderOption option(kind, length);

        EXPECT_EQ(kind, option.get_kind());
        EXPECT_EQ(length, option.get_length());
    }

    TEST(TCPHeaderOptionTest, SetGetKindTest) {
        u_int8_t kind = 1;
        u_int8_t length = 2;

        TCPHeaderOption option(kind, length);

        EXPECT_EQ(kind, option.get_kind());
        EXPECT_EQ(length, option.get_length());

        kind = 200;

        option.set_kind(kind);
        EXPECT_EQ(kind, option.get_kind());
    }

    TEST(TCPHeaderOptionTest, SetGetLengthTest) {
        u_int8_t kind = 1;
        u_int8_t length = 2;

        TCPHeaderOption option(kind, length);

        EXPECT_EQ(kind, option.get_kind());
        EXPECT_EQ(length, option.get_length());

        length = 20;

        option.set_length(length);
        EXPECT_EQ(length, option.get_length());
    }

    TEST(TCPHeaderOptionTest, GetSetValueTest) {
        u_int8_t kind = 1;
        u_int8_t length = 2;

        TCPHeaderOption option(kind, length);

        EXPECT_EQ(kind, option.get_kind());
        EXPECT_EQ(length, option.get_length());

        const char* value = "This is the value";
        option.set_value((unsigned char*)value, strlen(value));

        EXPECT_EQ(kind, option.get_kind());
        EXPECT_EQ(length, option.get_length());

        EXPECT_TRUE(!strncmp(value, (const char*) option.get_value(), strlen(value)));
    }

    TEST(TCPHeaderOptionTest, GetSetDataPointerTest) {
        u_int8_t kind = 1;
        u_int8_t length = 2;

        TCPHeaderOption option(kind, length);

        EXPECT_EQ(kind, option.get_kind());
        EXPECT_EQ(length, option.get_length());

        const char* value = "This is the value";
        option.set_value((unsigned char*)value, strlen(value));

        EXPECT_EQ(kind, option.get_kind());
        EXPECT_EQ(length, option.get_length());

        EXPECT_TRUE(!strncmp(value, (const char*) option.get_value(), strlen(value)));


        u_int8_t actual_kind = *option.get_data();
        EXPECT_EQ(kind, actual_kind);

        u_int8_t actual_length = *(option.get_data() + 1);
        EXPECT_EQ(length, actual_length);

        EXPECT_EQ(option.get_value(), option.get_data() + 2);

        const char* actual_value = (const char*) (option.get_data() + 2);
        EXPECT_TRUE(!strncmp(value, actual_value, strlen(value)));


        TCPHeaderOption other(0, 0);
        other.set_data(option.get_data(), 2 + strlen(value));

        EXPECT_EQ(kind, other.get_kind());
        EXPECT_EQ(length, other.get_length());
        EXPECT_TRUE(!strncmp(value, (const char*) other.get_value(), strlen(value)));
        actual_kind = *other.get_data();
        EXPECT_EQ(kind, actual_kind);
        actual_length = *(other.get_data() + 1);
        EXPECT_EQ(length, actual_length);
        EXPECT_EQ(option.get_value(), option.get_data() + 2);
        actual_value = (const char*) (other.get_data() + 2);
        EXPECT_TRUE(!strncmp(value, actual_value, strlen(value)));
    }
}

#endif