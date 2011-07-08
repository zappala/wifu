#include <iostream>
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
#include "../headers/packet/TCPPacket.h"

#include "../headers/BackEndTest.h"

#include "../headers/PacketTraceHelper.h"
#include "Utils.h"

void* active_to_passive_thread_with_close_multiple(void* args) {
    struct var* v = (struct var*) args;
    AddressPort* to_bind = v->to_bind_;
    Semaphore* sem = v->sem_;
    Semaphore* flag = v->flag_;
    Semaphore* done = v->done_;

    gcstring expected = v->expected_string;

    // Create server
    int server = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);
    int result = wifu_bind(server, (const struct sockaddr *) to_bind->get_network_struct_ptr(), sizeof (struct sockaddr_in));
    EXPECT_EQ(0, result);
    result = wifu_listen(server, 5);
    EXPECT_EQ(0, result);

    sem->post();

    struct sockaddr_in addr;
    socklen_t length = sizeof (addr);
    int connection;
    while ((connection = wifu_accept(server, (struct sockaddr *) & addr, &length)) > 0) {
        flag->post();

        AddressPort ap(&addr);
        gcstring address("127.0.0.1");
        gcstring res = ap.get_address();
        EXPECT_EQ(address, res);

        int size = 50000;
        char buffer[size];
        memset(buffer, 0, size);
        gcstring all_received = "";

        while (true) {
            int return_value = wifu_recv(connection, &buffer, 1, 0);
            if (return_value <= 0) {
                break;
            }

            gcstring actual(buffer);
            all_received.append(actual);
        }
        wifu_close(connection);
        EXPECT_EQ(expected, all_received);
    }
    wifu_close(server);
}

/**
 * @param num_bytes The number of bytes to send, currently, this is also the number of packets to send (we sent one data byte per packet)
 *
 */
void active_to_passive_test_with_close_multiple(gcstring message) {
    AddressPort to_connect("127.0.0.1", 5002);

    pthread_t t;
    struct var v;
    Timer timer;
    int client;
    int result;

    v.sem_ = new Semaphore();
    v.flag_ = new Semaphore();
    v.done_ = new Semaphore();
    v.sem_->init(0);
    v.flag_->init(0);
    v.done_->init(0);
    v.to_bind_ = new AddressPort("127.0.0.1", 5002);

    //Specify the number of bytes to send here.
    v.expected_string = message;

    if (pthread_create(&(t), NULL, &active_to_passive_thread_with_close_multiple, &(v)) != 0) {
        FAIL() << "Error creating new thread in IntegrationTest.h";
    }

    // ensure all variables are copied
    v.sem_->wait();

    for (int i = 0; i < 10; i++) {

        // Create client
        timer.start();
        client = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);
        result = wifu_connect(client, (const struct sockaddr *) to_connect.get_network_struct_ptr(), sizeof (struct sockaddr_in));
        timer.stop();
        ASSERT_EQ(0, result);

        v.flag_->wait();

        int size = 50000;
        char buffer[size];

        memcpy(buffer, message.c_str(), message.length());

        int count = 1;
        int num_sent = 0;

        for (int i = 0; i < message.length(); i++) {
            num_sent += wifu_send(client, &(buffer[i]), count, 0);
        }

        EXPECT_EQ(message.length(), num_sent);
        wifu_close(client);
    }
    sleep(2);
}

TEST_F(BackEndTest, sendReceiveTestActiveToPassiveWithCloseMultiple0) {
    active_to_passive_test_with_close_multiple(random_string(0));
}

TEST_F(BackEndTest, sendReceiveTestActiveToPassiveWithCloseMultiple5000) {
    active_to_passive_test_with_close_multiple(random_string(5000));
}

TEST_F(BackEndTest, sendReceiveTestActiveToPassiveWithCloseMultiple10000) {
    active_to_passive_test_with_close_multiple(random_string(10000));
}