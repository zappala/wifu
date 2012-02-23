#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/udp.h>

#include "gtest/gtest.h"
#include "../applib/wifu_socket.h"
#include "../headers/defines.h"

#include "../headers/BackEndTest.h"
#include "../headers/RandomStringGenerator.h"
#include "../headers/Utils.h"
#include <stdio.h>

TEST_F(BackEndTest, UDPSocketTest) {
    ASSERT_TRUE(sizeof (struct udphdr) == UDP_HEADER_LENGTH_BYTES);

    for (int i = 0; i < 100; i++) {
        // Check valid
        int socket = wifu_socket(AF_INET, SOCK_DGRAM, UDP);
        ASSERT_TRUE(socket >= 0);

        socket = wifu_socket(AF_INET, SOCK_DGRAM, i);
        ASSERT_TRUE(socket == -1);
    }
}

struct udp_struct {
    gcstring message;
    AddressPort* ap;
    int packets;
    Semaphore* ready;

};

void* udp_thread(void* args) {

    struct udp_struct* info = (struct udp_struct*) args;
    gcstring message = info->message;
    AddressPort* ap = new AddressPort(info->ap->get_address(), info->ap->get_port());
    int packets = info->packets;
    Semaphore* ready = info->ready;


    int socket = wifu_socket(AF_INET, SOCK_DGRAM, UDP);
    if (socket < 0) {
        perror("Error creating udp socket in thread");
        exit(EXIT_FAILURE);
    }

    socklen_t length = sizeof (struct sockaddr_in);

    if (wifu_bind(socket, (struct sockaddr*) ap->get_network_struct_ptr(), length) < 0) {
        perror("Error binding udp socket");
        exit(EXIT_FAILURE);
    }


    gcstring data = "";
    int size = 1500;
    char buffer[size];
    ready->post();
    for (int i = 0; i < packets; i++) {
        ssize_t n = wifu_recvfrom(socket, buffer, size, 0, (struct sockaddr*) ap->get_network_struct_ptr(), & length);
        data.append(buffer, n);
    }

    wifu_close(socket);
    EXPECT_EQ(message, data);

}

TEST_F(BackEndTest, UDPTransfer) {
    Semaphore* ready = new Semaphore();
    ready->init(0);
    int packets = 1000;
    int length = 1000 * packets;
    gcstring message = RandomStringGenerator::get_data(length);
    AddressPort* receiver = new AddressPort("127.0.0.1", 5000);

    struct udp_struct info;
    info.ap = receiver;
    info.message = message;
    info.packets = packets;
    info.ready = ready;

    pthread_t thread;
    if (pthread_create(&thread, NULL, &udp_thread, &info) != 0) {
        FAIL() << "Error creating thread in UDPTest";
    }


    int socket = wifu_socket(AF_INET, SOCK_DGRAM, UDP);
    if (socket < 0) {
        FAIL() << "Error creating udp socket";
    }

    socklen_t len = sizeof (struct sockaddr_in);

    int index = 0;
    int increment = 1000;
    int total_sent = 0;

    ready->wait();
    while (index < message.size()) {

        int n = wifu_sendto(socket, message.data() + index, increment, 0, (struct sockaddr*) receiver->get_network_struct_ptr(), len);
        total_sent += n;
        index += increment;

    }

    wifu_close(socket);

    ASSERT_EQ(message.size(), total_sent);

    pthread_join(thread, NULL);
}