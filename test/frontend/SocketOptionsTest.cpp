#ifndef SOCKETOPTIONSTEST
#define SOCKETOPTIONSTEST
#include <iostream>
#include <string>
#include <vector>
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

        int fd = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);

        int level = 0;
        int optname = 0;
        string optvalue = "hello";
        socklen_t optlen = optvalue.size();

        wifu_setsockopt(fd, level, optname, optvalue.c_str(), optlen);

        unsigned char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        socklen_t len = BUFFER_SIZE;

        wifu_getsockopt(fd, level, optname, buffer, &len);

        cout << "Buffer: " << buffer << endl;

        string result = (const char*) buffer;
        ASSERT_EQ(optvalue, result);
    }

    TEST_F(BackEndMockTestDropNone, socketOptionsTestIntValue) {

        int fd = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);

        int level = 0;
        int optname = 0;
        string optvalue = "hello";
        socklen_t optlen = optvalue.size();

        wifu_setsockopt(fd, level, optname, optvalue.c_str(), optlen);

        unsigned char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        socklen_t len = BUFFER_SIZE;

        wifu_getsockopt(fd, level, optname, buffer, &len);

        cout << "Buffer: " << buffer << endl;

        string result = (const char*) buffer;
        ASSERT_EQ(optvalue, result);
    }
}

#endif /*SOCKETOPTIONSTEST*/