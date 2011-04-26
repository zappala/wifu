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
        int socket = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);

        int optval = 1;
        socklen_t optlen = sizeof(optval);

        wifu_setsockopt(socket, SOL_TCP, TCP_KEEPCNT, &optval, optlen);
    }
}

#endif /*SOCKETOPTIONSTEST*/