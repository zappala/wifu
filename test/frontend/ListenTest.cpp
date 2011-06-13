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

TEST_F(BackEndTest, listenTest) {
    // Bad fd
    // (badfd may actually have been created above)
    int badfd = 4000;
    int queue_size = 5;
    int result = wifu_listen(badfd, queue_size);
    ASSERT_EQ(EBADF, errno);
    ASSERT_TRUE(result < 0);

    struct sockaddr_in to_bind;
    socklen_t length = sizeof (struct sockaddr_in);
    memset(&to_bind, 0, length);
    to_bind.sin_family = AF_INET;
    to_bind.sin_port = htons(5001);
    to_bind.sin_addr.s_addr = INADDR_ANY;

    // Should be successful
    int socket = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);
    result = wifu_bind(socket, (const struct sockaddr *) & to_bind, length);
    ASSERT_EQ(0, result);
    result = wifu_listen(socket, 5);
    ASSERT_EQ(0, result);

    // Should fail
    result = wifu_listen(socket, 5);
    ASSERT_TRUE(result < 0);
    ASSERT_EQ(EADDRINUSE, errno);
}