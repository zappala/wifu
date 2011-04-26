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

    TEST_F(BackEndMockTestDropNone, socketOptionsTestAll) {


        int fd = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);

        int level = 0;
        int optname = 0;
        int optval = 1;
        socklen_t optlen = sizeof(optval);

        wifu_setsockopt(fd, level, optname, &optval, optlen);

        unsigned char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        socklen_t len = BUFFER_SIZE;

        wifu_getsockopt(fd, level, optname, buffer, &len);

        ASSERT_EQ(optval, atoi((const char*) buffer));
        


    }
}

#endif /*SOCKETOPTIONSTEST*/