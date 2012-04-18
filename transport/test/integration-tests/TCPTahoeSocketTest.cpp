#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>

#include "gtest/gtest.h"
#include "../applib/wifu_socket.h"
#include "../headers/defines.h"

#include "../headers/WiFuTransportIntegrationTest.h"

TEST_F(WiFuTransportIntegrationTest, TCPTahoeSocketTest) {

    for (int i = 0; i < 100; i++) {
        // Check valid
        int socket = wifu_socket(AF_INET, SOCK_STREAM, TCP_TAHOE);
        ASSERT_TRUE(socket >= 0);

        // Check invalid (i != TCP_TAHOE)
        socket = wifu_socket(AF_INET, SOCK_STREAM, i);
        ASSERT_EQ(-1, socket);
    }
}
