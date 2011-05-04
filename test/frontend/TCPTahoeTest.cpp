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
#include "../headers/defines.h"
#include "../headers/AddressPort.h"
#include "../headers/GarbageCollector.h"
#include "../headers/Semaphore.h"
#include "Timer.h"
#include "../headers/RandomStringGenerator.h"

#include "../headers/BackEndTest.h"

TEST_F(BackEndTest, TCPTahoeSocketTest) {

    for (int i = 0; i < 100; i++) {
        // Check valid
        int socket = wifu_socket(AF_INET, SOCK_STREAM, TCP_TAHOE);
        ASSERT_TRUE(socket >= 0);

        // Check invalid (i != TCP_TAHOE)
        socket = wifu_socket(AF_INET, SOCK_STREAM, i);
        ASSERT_TRUE(socket == -1);
    }
}