#ifndef SOCKETOPTIONSTEST
#define SOCKETOPTIONSTEST
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>

#include "gtest/gtest.h"
#include "../applib/wifu_socket.h"
#include "../headers/BackEndTest.h"
#include "defines.h"
#include <netinet/tcp.h>

using namespace std;

namespace {

    TEST_F(BackEndMockTestDropNone, socketOptionsTestCharacterValue) {
        int fd = wifu_socket(AF_INET, SOCK_STREAM, TCP_TAHOE);

        int level = 0;
        int optname = 0;
        gcstring optvalue = "hello";
        socklen_t optlen = optvalue.size();

        int return_val = wifu_setsockopt(fd, level, optname, optvalue.c_str(), optlen);

        EXPECT_EQ(0, return_val);

        unsigned char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        socklen_t len = BUFFER_SIZE;

        return_val = wifu_getsockopt(fd, level, optname, buffer, &len);
        gcstring result = (const char*) buffer;

        EXPECT_EQ(0, return_val);
        EXPECT_EQ(optvalue, result);
        EXPECT_EQ(optlen, len);
    }

    TEST_F(BackEndMockTestDropNone, socketOptionsTestIntValue) {
        int fd = wifu_socket(AF_INET, SOCK_STREAM, TCP_TAHOE);

        int level = 0;
        int optname = 0;
        int optvalue = 50;
        socklen_t optlen = sizeof (optvalue);

        int return_val = wifu_setsockopt(fd, level, optname, &optvalue, optlen);
        EXPECT_EQ(0, return_val);

        unsigned char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        socklen_t len = BUFFER_SIZE;

        return_val = wifu_getsockopt(fd, level, optname, buffer, &len);
        int* result = (int*) buffer;

        EXPECT_EQ(0, return_val);
        EXPECT_EQ(optvalue, *result);
        EXPECT_EQ(len, optlen);
    }

    TEST_F(BackEndMockTestDropNone, socketOptionsTestDoubleValue) {
        int fd = wifu_socket(AF_INET, SOCK_STREAM, TCP_TAHOE);

        int level = 0;
        int optname = 0;
        double optvalue = rand() / RAND_MAX;
        socklen_t optlen = sizeof (optvalue);

        int return_val = wifu_setsockopt(fd, level, optname, &optvalue, optlen);
        EXPECT_EQ(0, return_val);

        unsigned char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        socklen_t len = BUFFER_SIZE;

        return_val = wifu_getsockopt(fd, level, optname, buffer, &len);
        double* result = (double*) buffer;

        EXPECT_EQ(0, return_val);
        EXPECT_EQ(optvalue, *result);
        EXPECT_EQ(len, optlen);
    }

    TEST_F(BackEndMockTestDropNone, socketOptionsTestBoolValue) {
        int fd = wifu_socket(AF_INET, SOCK_STREAM, TCP_TAHOE);

        int level = 0;
        int optname = 0;
        int optvalue = 1;
        socklen_t optlen = sizeof (optvalue);

        int return_val = wifu_setsockopt(fd, level, optname, &optvalue, optlen);
        EXPECT_EQ(0, return_val);

        unsigned char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        socklen_t len = BUFFER_SIZE;

        return_val = wifu_getsockopt(fd, level, optname, buffer, &len);
        int* result = (int*) buffer;

        EXPECT_EQ(0, return_val);
        EXPECT_EQ(optvalue, *result);
        EXPECT_EQ(len, optlen);
    }

    TEST_F(BackEndMockTestDropNone, socketOptionsTestTimestampValue) {
        int fd = wifu_socket(AF_INET, SOCK_STREAM, TCP_TAHOE);

        for (int i = 0; i < 1000; i++) {

            int optvalue = rand() % 2;
            socklen_t optlen = sizeof (optvalue);

            int return_val = wifu_setsockopt(fd, SOL_TCP, TCP_TIMESTAMP, &optvalue, optlen);
            EXPECT_EQ(0, return_val);

            unsigned char buffer[BUFFER_SIZE];
            memset(buffer, 0, BUFFER_SIZE);
            socklen_t len = BUFFER_SIZE;

            return_val = wifu_getsockopt(fd, SOL_TCP, TCP_TIMESTAMP, buffer, &len);
            int* result = (int*) buffer;

            EXPECT_EQ(0, return_val);
            EXPECT_EQ(optvalue, *result);
            EXPECT_EQ(len, optlen);

        }
    }

    TEST_F(BackEndMockTestDropNone, socketOptionsTestCharStar) {
        int fd = wifu_socket(AF_INET, SOCK_STREAM, TCP_TAHOE);

        for (int i = 0; i < 1000; i++) {
            gcstring optval = rand() % 2 == 1 ? "on" : "off";
            socklen_t optlen = optval.length();

            int return_val = wifu_setsockopt(fd, SOL_TCP, 5, optval.c_str(), optlen);
            EXPECT_EQ(0, return_val);

            unsigned char buffer[BUFFER_SIZE];
            memset(buffer, 0, BUFFER_SIZE);
            socklen_t len = BUFFER_SIZE;

            return_val = wifu_getsockopt(fd, SOL_TCP, 5, buffer, &len);

            gcstring actual((const char*) buffer);
            EXPECT_EQ(0, return_val);
            EXPECT_EQ(optval, actual);
            EXPECT_EQ(len, optlen);
        }
    }

    TEST_F(BackEndMockTestDropNone, socketOptionsTestManyIntValues) {
        // This code fails

        //        int fd = wifu_socket(AF_INET, SOCK_STREAM, TCP_TAHOE);
        //        int max = 18;
        //        unsigned char buffer[BUFFER_SIZE];
        //
        //
        //        for (int level = 0; level < max; level++) {
        //            for (int optname = 0; optname < max; optname++) {
        //                int optvalue = level * optname;
        //
        //                cout << "level: " << level << endl;
        //                cout << "optname: " << optname << endl;
        //                cout << "optvalue: " << optvalue << endl;
        //
        //                socklen_t optlen = sizeof (optvalue);
        //
        //                int return_val = wifu_setsockopt(fd, level, optname, &optvalue, optlen);
        //                EXPECT_EQ(0, return_val);
        //            }
        //        }
        //
        //        cout << "Reading" << endl;
        //
        //        for (int level = 0; level < max; level++) {
        //            for (int optname = 0; optname < max; optname++) {
        //
        //                int optvalue = level * optname;
        //
        //                cout << "level: " << level << endl;
        //                cout << "optname: " << optname << endl;
        //                cout << "optvalue: " << optvalue << endl;
        //
        //
        //                memset(buffer, 0, BUFFER_SIZE);
        //                socklen_t len = BUFFER_SIZE;
        //
        //                int return_val = wifu_getsockopt(fd, level, optname, buffer, &len);
        //                int* result = (int*) buffer;
        //
        //                EXPECT_EQ(0, return_val);
        //                EXPECT_EQ(optvalue, *result);
        //                EXPECT_EQ(sizeof (socklen_t), len);
        //            }
        //        }
    }
}

#endif /*SOCKETOPTIONSTEST*/