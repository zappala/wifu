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
#include "../headers/packet/UDPPacket.h"
#include <sys/socket.h>

#include "../headers/BackEndTest.h"

#include "../headers/PacketTraceHelper.h"
#include "Utils.h"

void* udp_active_to_passive_thread_with_close(void* args) {


    struct udpvar* v = (struct udpvar*) args;
    AddressPort* to_bind = v->to_bind_;
    Semaphore* sem = v->sem_;
    Semaphore* done = v->done_;
    int countdown = v->countdown_;
    int rec_buf = v->rec_buf_;

    gcstring expected = v->expected_string;

    // Create server
    int server = wifu_socket(AF_INET, SOCK_DGRAM, UDP);
    cout << "UDP: Socket created.\n";
    int result = wifu_bind(server, (const struct sockaddr *) to_bind->get_network_struct_ptr(), sizeof (struct sockaddr_in));
    cout << "UDP: Bind called.\n";
    EXPECT_EQ(0, result);
    //result = wifu_listen(server, 5);
    //cout << "UDP: Listening...\n";
    //EXPECT_EQ(0, result);

    sem->post();

    /*AddressPort ap(&addr);
    gcstring address("127.0.0.1");
    gcstring res = ap.get_address();
    EXPECT_EQ(address, res);*/
    //    cout << "Connected to: " << ap.to_s() << endl;

    // TODO: Check the results of wifu_accept, probably need to wait for send, recv to be implemented

    int size = 500000;
    char buffer[size];
    gcstring all_received = "";
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    cout << "Countdown start: " << countdown << endl;

    while (true) {
        memset(buffer, 0, size);
        //cout << "UDP test: trying to receive...\n";
        int return_value = wifu_recvfrom(server, &buffer, rec_buf, 0, (struct sockaddr *)&addr, &addrlen);
        //cout << "UDP test: retval from recvfrom is " << return_value << ".\n";

        gcstring actual(buffer);
        all_received.append(actual);

        //cout << "So far, we got: " << all_received << endl;
        countdown--;
        //cout << "Countdown = " << countdown << endl;

        if (countdown == 0) {
            //cout << "Close Thread BREAK" << endl;
            break;
        }
    }
    //wifu_close(connection);
    wifu_close(server);
    EXPECT_EQ(expected, all_received);
    done->post();
    //    cout << "Received: " << all_received << endl;
}

/**
 * @param num_bytes The number of bytes to send, currently, this is also the number of packets to send (we sent one data byte per packet)
 *
 */
void udp_active_to_passive_test_with_close(gcstring message) {
    AddressPort to_connect("127.0.0.1", 5002);

    pthread_t t;
    struct udpvar v;
    Timer timer;
    int client;
    int result;

    v.sem_ = new Semaphore();
    v.done_ = new Semaphore();
    v.sem_->init(0);
    v.done_->init(0);
    v.to_bind_ = new AddressPort("127.0.0.1", 5002);
    v.rec_buf_ = 1;

    //Specify the number of bytes to send here.
    v.expected_string = message;
    v.countdown_ = message.length();

    cout << "UDP: Trying to create listener...\n";
    if (pthread_create(&(t), NULL, &udp_active_to_passive_thread_with_close, &(v)) != 0) {
        FAIL() << "Error creating new thread in IntegrationTest.h";
    }
    cout << "UDP: Listening socket created, waiting on v.sem_ post...\n";
    // ensure all variables are copied
    v.sem_->wait();

    cout << "UDP: Sleeping...\n";
    // ensure we are in accept
    usleep(50000);

    cout << "UDP: I'm awake!\n";
    // Create client
    timer.start();
    client = wifu_socket(AF_INET, SOCK_DGRAM, UDP);
    result = wifu_connect(client, (const struct sockaddr *) to_connect.get_network_struct_ptr(), sizeof (struct sockaddr_in));
    timer.stop();
    ASSERT_EQ(0, result);

    //cout << "UDP: Client socket created, waiting for v.flag_ post...\n";
    //v.flag_->wait();

    cout << "Duration (us) to create a socket and connect on localhost via wifu: " << timer.get_duration_microseconds() << endl;

    int size = 500000;
    char buffer[size];

    memcpy(buffer, message.c_str(), message.length());

    int count = 1;
    int num_sent = 0;

    struct sockaddr_in* addr = to_connect.get_network_struct_ptr();
    //u_int32_t address = (u_int32_t)addr->sin_addr;
    gcstring address = to_connect.get_address();
    u_int16_t port = (u_int16_t)addr->sin_port;

    //cout << "Dest. address: " << address << endl;
    //cout << "Dest. port: " << port << endl;

    //socklen_t addrlen = sizeof(addr);
    // TODO: this only sends one character at a time
    for (int i = 0; i < message.length(); i++) {
        //cout << "Sending " << i << endl;
        //cout << "which is " << message << endl;
        num_sent += wifu_sendto(client, &(buffer[i]), count, 0, (sockaddr*) addr, sizeof(struct sockaddr_in));
        //num_sent += wifu_send(client, &(buffer[i]), count, 0);
    }

    EXPECT_EQ(message.length(), num_sent);

    //    cout << "Sent: " << message << endl;

    //wifu_close(client);
    v.done_->wait();
    sleep(1);
}

/**
 * @param message The message that should arrive
 *
 */
void udp_active_to_passive_test_with_drop(gcstring message) {
    AddressPort to_connect("127.0.0.1", 5002);

    gcstring dummymsg = "This should not arrive.";
    pthread_t t;
    struct udpvar v;
    Timer timer;
    int client;
    int result;

    v.sem_ = new Semaphore();
    v.done_ = new Semaphore();
    v.sem_->init(0);
    v.done_->init(0);
    v.to_bind_ = new AddressPort("127.0.0.1", 5002);
    v.rec_buf_ = 500000;

    //Specify the number of bytes to send here.
    v.expected_string = message;
    v.countdown_ = 1;

    cout << "UDP: Trying to create listener...\n";
    if (pthread_create(&(t), NULL, &udp_active_to_passive_thread_with_close, &(v)) != 0) {
        FAIL() << "Error creating new thread in IntegrationTest.h";
    }
    cout << "UDP: Listening socket created, waiting on v.sem_ post...\n";
    // ensure all variables are copied
    v.sem_->wait();

    cout << "UDP: Sleeping...\n";
    // ensure we are in accept
    usleep(50000);

    cout << "UDP: I'm awake!\n";
    // Create client
    timer.start();
    client = wifu_socket(AF_INET, SOCK_DGRAM, UDP);
    result = wifu_connect(client, (const struct sockaddr *) to_connect.get_network_struct_ptr(), sizeof (struct sockaddr_in));
    timer.stop();
    ASSERT_EQ(0, result);

    //cout << "UDP: Client socket created, waiting for v.flag_ post...\n";
    //v.flag_->wait();

    cout << "Duration (us) to create a socket and connect on localhost via wifu: " << timer.get_duration_microseconds() << endl;

    int size = 500000;
    char buffer[size];

    memcpy(buffer, dummymsg.c_str(), dummymsg.length());

    //int count = 1;
    int num_sent = 0;

    struct sockaddr_in* addr = to_connect.get_network_struct_ptr();
    //u_int32_t address = (u_int32_t)addr->sin_addr;
    //gcstring address = to_connect.get_address();
    //u_int16_t port = (u_int16_t)addr->sin_port;

    //cout << "Dest. address: " << address << endl;
    //cout << "Dest. port: " << port << endl;

    num_sent += wifu_sendto(client, &(buffer[0]), dummymsg.length(), 0, (sockaddr*) addr, sizeof(struct sockaddr_in));

    usleep(500000);

    memcpy(buffer, message.c_str(), message.length());

    //socklen_t addrlen = sizeof(addr);
    // TODO: this only sends one character at a time
    //for (int i = 0; i < message.length(); i++) {
        //cout << "Sending " << i << endl;
        //cout << "which is " << message << endl;
        //num_sent += wifu_send(client, &(buffer[i]), count, 0);
    //}

    num_sent += wifu_sendto(client, &(buffer[0]), message.length(), 0, (sockaddr*) addr, sizeof(struct sockaddr_in));

    EXPECT_EQ(message.length() + dummymsg.length(), num_sent);

    //    cout << "Sent: " << message << endl;

    //wifu_close(client);
    v.done_->wait();
    sleep(1);
}

TEST_F(BackEndMockTestDropNone, UDPSendReceiveTestActiveToPassive1) {
    udp_active_to_passive_test_with_close(random_string(1));
}

TEST_F(BackEndMockTestDropNone, UDPSendReceiveTestActiveToPassive5) {
    udp_active_to_passive_test_with_close(random_string(5));
}

TEST_F(BackEndMockTestDropNone, UDPSendReceiveTestActiveToPassive10) {
    udp_active_to_passive_test_with_close(random_string(10));
}

TEST_F(BackEndMockTestDropNone, UDPSendReceiveTestActiveToPassive20) {
    udp_active_to_passive_test_with_close(random_string(20));
}

TEST_F(BackEndMockTestDropNone, UDPSendReceiveTestActiveToPassive100) {
    udp_active_to_passive_test_with_close(random_string(100));
}

TEST_F(BackEndMockTestDropNone, UDPSendReceiveTestActiveToPassive1000) {
    udp_active_to_passive_test_with_close(random_string(1000));
}

TEST_F(BackEndMockTestDropNone, UDPSendReceiveTestActiveToPassive2000) {
    udp_active_to_passive_test_with_close(random_string(2000));
}

TEST_F(BackEndMockTestDropNone, UDPSendReceiveTestActiveToPassive5000) {
    udp_active_to_passive_test_with_close(random_string(5000));
}

TEST_F(BackEndMockTestDropNone, UDPSendReceiveTestActiveToPassive50000) {
    udp_active_to_passive_test_with_close(random_string(50000));
}

TEST_F(BackEndMockTestDropNone, UDPSendReceiveTestActiveToPassive100000) {
    udp_active_to_passive_test_with_close(random_string(100000));
}

TEST_F(BackEndTest, UDPSendReceiveTestActiveToPassive1) {
    udp_active_to_passive_test_with_close(random_string(1));
}

TEST_F(BackEndTest, UDPSendReceiveTestActiveToPassive5) {
    udp_active_to_passive_test_with_close(random_string(5));
}

TEST_F(BackEndTest, UDPSendReceiveTestActiveToPassive10) {
    udp_active_to_passive_test_with_close(random_string(10));
}

TEST_F(BackEndTest, UDPSendReceiveTestActiveToPassive20) {
    udp_active_to_passive_test_with_close(random_string(20));
}

TEST_F(BackEndTest, UDPSendReceiveTestActiveToPassive100) {
    udp_active_to_passive_test_with_close(random_string(100));
}

TEST_F(BackEndTest, UDPSendReceiveTestActiveToPassive1000) {
    udp_active_to_passive_test_with_close(random_string(1000));
}

//For some reason, this test (not the guts of it, but the TEST_F line)
//causes the test tahoeSendReceiveTestPassiveToActiveBigChunks10000000
//in TCPTahoeSendReceivePassiveToActiveBigChunks.cpp to segfault
//when creating the (very large) random string if we don't reserve the
//memory using gcstring.reserve() first.
//There can be only one?
//TEST_F(BackEndMockTestDrop10, UDPDropFirstPacketTest) {
//    udp_active_to_passive_test_with_drop("Hier stehe ich; ich kann nichts anders!");
//    udp_active_to_passive_test_with_drop(random_string(100));
//}