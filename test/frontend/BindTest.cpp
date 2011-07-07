#include <iostream>
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

TEST_F(BackEndTest, bindTest) {
    struct sockaddr_in to_bind;
    socklen_t length = sizeof (struct sockaddr_in);
    memset(&to_bind, 0, length);

    to_bind.sin_family = AF_INET;
    to_bind.sin_port = htons(5000);
    //TODO: do we want to ever bind on any other address?
    to_bind.sin_addr.s_addr = INADDR_ANY;

    // Bad fd
    //TODO: need to close all the sockets opened up above as we may collide
    // (badfd may actually have been created above)
    int badfd = 4000;
    int result = wifu_bind(badfd, (const struct sockaddr *) & to_bind, length);
    ASSERT_EQ(EBADF, errno);
    ASSERT_TRUE(result < 0);

    // Should be successful
    int socket = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);
    result = wifu_bind(socket, (const struct sockaddr *) & to_bind, length);
    ASSERT_EQ(0, result);


    // Should fail (already bound)
    result = wifu_bind(socket, (const struct sockaddr *) & to_bind, length);
    ASSERT_TRUE(result < 0);
    ASSERT_EQ(EINVAL, errno);
}