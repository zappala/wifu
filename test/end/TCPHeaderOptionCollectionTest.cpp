#ifndef _TCPHEADEROPTIONCOLLECTIONTEST
#define _TCPHEADEROPTIONCOLLECTIONTEST

#include "gtest/gtest.h"
#include "packet/TCPHeaderOptionCollection.h"

using namespace std;

namespace {

    TEST(TCPHeaderOptionCollectionTest, InsertContainsTest) {
        TCPHeaderOptionCollection c;

        EXPECT_TRUE(c.empty());

        u_int8_t kind = 10;
        u_int8_t length = 5;
        TCPHeaderOption* option = new TCPHeaderOption(kind, length);
        c.insert(option);

        EXPECT_FALSE(c.empty());
        EXPECT_TRUE(c.contains(kind));
    }

    TEST(TCPHeaderOptionCollectionTest, InsertRemoveTest) {
        TCPHeaderOptionCollection c;

        EXPECT_TRUE(c.empty());

        u_int8_t kind = 10;
        u_int8_t length = 5;
        TCPHeaderOption* option = new TCPHeaderOption(kind, length);
        c.insert(option);

        EXPECT_FALSE(c.empty());

        TCPHeaderOption* actual = c.remove(kind);
        EXPECT_TRUE(c.empty());
        EXPECT_EQ(option, actual);
    }

    TEST(TCPHeaderOptionCollectionTest, ParseTwoOptions) {
        TCPHeaderOptionCollection c;

        EXPECT_TRUE(c.empty());

        unsigned char buffer[50];
        unsigned char* ptr = buffer;

        u_int8_t kind = 5;
        u_int8_t length = 3;
        const char* value = "a";

        memcpy(ptr, &kind, 1);
        memcpy(ptr + 1, &length, 1);
        memcpy(ptr + 2, value, strlen(value));

        // move buffer up three spots;
        ptr += length;

        kind = 6;
        length = 4;
        value = "bc";
        memcpy(ptr, &kind, 1);
        memcpy(ptr + 1, &length, 1);
        memcpy(ptr + 2, value, strlen(value));

        ptr += length;
        // add in padding
        memset(ptr, 0, 1);

        ptr -= 7;
        c.parse(ptr, 8);

        EXPECT_FALSE(c.empty());
        EXPECT_TRUE(c.contains(5));
        EXPECT_TRUE(c.contains(6));

        TCPHeaderOption* a = c.remove(6);
        EXPECT_EQ(6, a->get_kind());
        EXPECT_EQ(4, a->get_length());
        EXPECT_TRUE(!memcmp(value, a->get_value(), 2));

        TCPHeaderOption* b = c.remove(5);
        value = "a";
        EXPECT_EQ(5, b->get_kind());
        EXPECT_EQ(3, b->get_length());
        EXPECT_TRUE(!memcmp(value, b->get_value(), 1));
    }

        TEST(TCPHeaderOptionCollectionTest, ParseTwoOptionsNoPadding) {
        TCPHeaderOptionCollection c;

        EXPECT_TRUE(c.empty());

        unsigned char buffer[50];
        unsigned char* ptr = buffer;

        u_int8_t kind = 5;
        u_int8_t length = 4;
        const char* value = "ab";

        memcpy(ptr, &kind, 1);
        memcpy(ptr + 1, &length, 1);
        memcpy(ptr + 2, value, strlen(value));

        // move buffer up three spots;
        ptr += length;

        kind = 6;
        length = 4;
        value = "cd";
        memcpy(ptr, &kind, 1);
        memcpy(ptr + 1, &length, 1);
        memcpy(ptr + 2, value, strlen(value));

        ptr += length;
        // add in padding
        memset(ptr, 0, 1);

        ptr -= 8;
        c.parse(ptr, 8);

        EXPECT_FALSE(c.empty());
        EXPECT_TRUE(c.contains(5));
        EXPECT_TRUE(c.contains(6));

        TCPHeaderOption* a = c.remove(6);
        EXPECT_EQ(6, a->get_kind());
        EXPECT_EQ(4, a->get_length());
        EXPECT_TRUE(!memcmp(value, a->get_value(), 2));

        TCPHeaderOption* b = c.remove(5);
        value = "ab";
        EXPECT_EQ(5, b->get_kind());
        EXPECT_EQ(4, b->get_length());
        EXPECT_TRUE(!memcmp(value, b->get_value(), 2));
    }
}

#endif /*_TCPHEADEROPTIONCOLLECTIONTEST*/
