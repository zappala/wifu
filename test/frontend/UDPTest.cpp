#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/udp.h>

#include "gtest/gtest.h"
#include "../applib/wifu_socket.h"
#include "../headers/defines.h"

#include "../headers/BackEndTest.h"

TEST_F(BackEndTest, UDPSocketTest) {
    ASSERT_TRUE(sizeof(struct udphdr) == UDP_HEADER_LENGTH_BYTES);

    for (int i = 0; i < 100; i++) {
        // Check valid
        int socket = wifu_socket(AF_INET, SOCK_DGRAM, UDP);
        ASSERT_TRUE(socket >= 0);

        // Check invalid (i != TCP_TAHOE)
        socket = wifu_socket(AF_INET, SOCK_DGRAM, i);
        ASSERT_TRUE(socket == -1);
    }
}

TEST_F(BackEndTest, UDPSendTest) {
    
}

TEST_F(BackEndTest, UDPReceiveTest) {
    
}