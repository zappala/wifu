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
#include "../headers/packet/TCPPacket.h"

#include "../headers/BackEndTest.h"

#include "../headers/PacketTraceHelper.h"
#include "Utils.h"

void* tahoe_active_to_passive_thread_with_close(void* args) {


    struct var* v = (struct var*) args;
    AddressPort* to_bind = v->to_bind_;
    Semaphore* sem = v->sem_;
    Semaphore* flag = v->flag_;
    Semaphore* done = v->done_;

    string expected = v->expected_string;

    // Create server
    int server = wifu_socket(AF_INET, SOCK_STREAM, TCP_TAHOE);
    int result = wifu_bind(server, (const struct sockaddr *) to_bind->get_network_struct_ptr(), sizeof (struct sockaddr_in));
    EXPECT_EQ(0, result);
    result = wifu_listen(server, 5);
    EXPECT_EQ(0, result);

    sem->post();

    struct sockaddr_in addr;
    socklen_t length = sizeof (addr);
    int connection;
    if ((connection = wifu_accept(server, (struct sockaddr *) & addr, &length)) < 0) {
        // TODO: we need to check errors and make sure they happen when they should
        ADD_FAILURE() << "Problem in Accept";
    }

    flag->post();



    AddressPort ap(&addr);
    string address("127.0.0.1");
    string res = ap.get_address();
    EXPECT_EQ(address, res);
//    cout << "Connected to: " << ap.to_s() << endl;

    // TODO: Check the results of wifu_accept, probably need to wait for send, recv to be implemented

    int size = 50000;
    char buffer[size];
    memset(buffer, 0, size);
    string all_received = "";

    while (true) {
        int return_value = wifu_recv(connection, &buffer, 1, 0);

        if (return_value == 0) {
//            cout << "Close Thread BREAK" << endl;
            break;
        }

        string actual(buffer);
        all_received.append(actual);
    }
    wifu_close(connection);
    wifu_close(server);
    EXPECT_EQ(expected, all_received);
    done->post();
//    cout << "Received: " << all_received << endl;
}

/**
 * @param num_bytes The number of bytes to send, currently, this is also the number of packets to send (we sent one data byte per packet)
 *
 */
void tahoe_active_to_passive_test_with_close(string message) {
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



    if (pthread_create(&(t), NULL, &tahoe_active_to_passive_thread_with_close, &(v)) != 0) {
        FAIL() << "Error creating new thread in IntegrationTest.h";
    }

    // ensure all variables are copied
    v.sem_->wait();

    // ensure we are in accept
    usleep(50000);

    // Create client
    timer.start();
    client = wifu_socket(AF_INET, SOCK_STREAM, TCP_TAHOE);
    result = wifu_connect(client, (const struct sockaddr *) to_connect.get_network_struct_ptr(), sizeof (struct sockaddr_in));
    timer.stop();
    ASSERT_EQ(0, result);

    v.flag_->wait();

    cout << "Duration (us) to create a socket and connect on localhost via wifu: " << timer.get_duration_microseconds() << endl;

    int size = 50000;
    char buffer[size];

    memcpy(buffer, message.c_str(), message.length());

    int count = 1;
    int num_sent = 0;

    // TODO: this only sends one character at a time
    for (int i = 0; i < message.length(); i++) {
//        cout << "Sending" << endl;
        num_sent += wifu_send(client, &(buffer[i]), count, 0);
    }

    EXPECT_EQ(message.length(), num_sent);

//    cout << "Sent: " << message << endl;

    wifu_close(client);
    v.done_->wait();
    sleep(1);
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestActiveToPassive1) {
    tahoe_active_to_passive_test_with_close(random_string(1));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestActiveToPassive5) {
    tahoe_active_to_passive_test_with_close(random_string(5));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestActiveToPassive10) {
    tahoe_active_to_passive_test_with_close(random_string(10));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestActiveToPassive20) {
    tahoe_active_to_passive_test_with_close(random_string(20));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestActiveToPassive100) {
    tahoe_active_to_passive_test_with_close(random_string(100));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestActiveToPassive1000) {
    tahoe_active_to_passive_test_with_close(random_string(1000));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestActiveToPassive2000) {
    tahoe_active_to_passive_test_with_close(random_string(2000));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestActiveToPassive5000) {
    tahoe_active_to_passive_test_with_close(random_string(5000));
}

TEST_F(BackEndMockTestDropNone, tahoeSendReceiveTestActiveToPassive50000) {
    tahoe_active_to_passive_test_with_close(random_string(50000));
}

TEST_F(BackEndMockTestDropRandom1Percent, tahoeSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(random_string(1000));
}

TEST_F(BackEndMockTestDropRandom5Percent, tahoeSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(random_string(1000));
}

TEST_F(BackEndMockTestDropRandom10Percent, tahoeSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(random_string(2000));
}

TEST_F(BackEndMockTestDropRandom20Percent, tahoeSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(random_string(2000));
}

TEST_F(BackEndMockTestDropRandom30Percent, tahoeSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(random_string(20000));
}

TEST_F(BackEndMockTestDropRandom40Percent, tahoeSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(random_string(20000));
}

TEST_F(BackEndMockTestDropRandom50Percent, tahoeSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(random_string(20000));
}

TEST_F(BackEndMockTestDropRandom60Percent, tahoeSendReceiveTestActiveToPassiveDropRandom) {
    tahoe_active_to_passive_test_with_close(random_string(20000));
}
