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

#include "../headers/PacketTraceHelper.h"
#include "Utils.h"

void* send_receive_thread(void* args) {

    struct var* v = (struct var*) args;
    AddressPort* to_bind = v->to_bind_;
    Semaphore* sem = v->sem_;

    string message = v->expected_string;

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
    if ((connection = wifu_accept(server, (struct sockaddr *) & addr, &length)) < 0) {
        // TODO: we need to check errors and make sure they happen when they should
        ADD_FAILURE() << "Problem in Accept";
    }

    AddressPort ap(&addr);
    string address("127.0.0.1");
    string res = ap.get_address();
    EXPECT_EQ(address, res);
    cout << "Connected to: " << ap.to_s() << endl;

    // TODO: Check the results of wifu_accept, probably need to wait for send, recv to be implemented

    int size = 50000;
    char buffer[size];

    memcpy(buffer, message.c_str(), message.length());

    int count = 1;
    int num_sent = 0;
    // TODO: this only sends one character at a time
    for (int i = 0; i < message.length(); i++) {

        num_sent += wifu_send(connection, &(buffer[i]), count, 0);
    }

    EXPECT_EQ(message.length(), num_sent);
    cout << "SendReceivePassiveToActive, sent message: " << message << endl;
}

/**
 * @param num_bytes The number of bytes to send, currently, this is also the number of packets to send (we sent one data byte per packet)
 *
 */
void send_receive_test(string message) {
    AddressPort to_connect("127.0.0.1", 5002);

    pthread_t t;
    struct var v;
    Timer timer;
    int client;
    int result;

    v.sem_ = new Semaphore();
    v.sem_->init(0);
    v.to_bind_ = new AddressPort("127.0.0.1", 5002);

    //Specify the number of bytes to send here.
    v.expected_string = message;


    if (pthread_create(&(t), NULL, &send_receive_thread, &(v)) != 0) {
        FAIL() << "Error creating new thread in IntegrationTest.h";
    }

    v.sem_->wait();

    // Make sure that the thread is in the accept state
    usleep(50000);

    // Create client

    timer.start();
    client = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);
    result = wifu_connect(client, (const struct sockaddr *) to_connect.get_network_struct_ptr(), sizeof (struct sockaddr_in));
    timer.stop();
    EXPECT_EQ(0, result);

    cout << "Duration (us) to create a socket and connect on localhost via wifu: " << timer.get_duration_microseconds() << endl;

    int size = 1500;
    char buffer[size];
    memset(buffer, 0, size);
    string expected = v.expected_string;
    string all_received = "";

    for (int count = 0; count < expected.length(); ++count) {

        string exp = expected.substr(count, 1);
        int return_value = wifu_recv(client, &buffer, 1, 0);
        string actual(buffer);
        all_received.append(actual);


        EXPECT_EQ(1, return_value);
        EXPECT_EQ(exp, actual);

    }

    EXPECT_EQ(expected, all_received);

    cout << "IntegrationTest::send_receive_test(), received the following: " << all_received << endl;
    sleep(5);
}

TEST_F(BackEndMockTestDropNone, sendReceiveTestPassiveToActive1) {
    send_receive_test(random_string(1));
}

TEST_F(BackEndMockTestDropNone, sendReceiveTestPassiveToActive10) {
    send_receive_test(random_string(10));
}

TEST_F(BackEndMockTestDropNone, sendReceiveTestPassiveToActive100) {
    send_receive_test(random_string(100));
}

TEST_F(BackEndMockTestDropNone, sendReceiveTestPassiveToActive1000) {
    send_receive_test(random_string(1000));
}

TEST_F(BackEndMockTestDropNone, sendReceiveTestPassiveToActive2000) {
    send_receive_test(random_string(2000));
}

TEST_F(BackEndMockTestDropNone, sendReceiveTestPassiveToActive5000) {
    send_receive_test(random_string(5000));
}

TEST_F(BackEndMockTestDropNone, sendReceiveTestPassiveToActive50000) {
    send_receive_test(random_string(50000));
}

void drop_ack_send_data_passive_to_active() {
    string data = random_string(1);

    send_receive_test(data);

    NetworkTrace expected;

    // Send
    expected.add_packet(get_syn());
    // receive
    expected.add_packet(get_syn());

    // send
    expected.add_packet(get_synack());
    // receive
    expected.add_packet(get_synack());

    // send
    expected.add_packet(get_ack());

    // resend
    expected.add_packet(get_synack());
    // receive
    expected.add_packet(get_synack());

    // send
    expected.add_packet(get_ack());
    // receive
    expected.add_packet(get_ack());


    TCPPacket* data_packet = get_base_tcp_packet();
    data_packet->set_data((unsigned char*) data.c_str(), data.size());
    data_packet->set_tcp_sequence_number(2);
    data_packet->set_tcp_ack_number(3);
    data_packet->set_tcp_ack(true);
    data_packet->set_destination_port(1000);
    data_packet->set_source_port(5002);

    // send
    expected.add_packet(data_packet);
    // receive
    expected.add_packet(data_packet);

    TCPPacket* ack = get_base_tcp_packet();
    ack->set_tcp_sequence_number(3);
    ack->set_tcp_ack_number(3);
    ack->set_source_port(1000);
    ack->set_destination_port(5002);
    ack->set_tcp_ack(true);

    // send
    expected.add_packet(ack);
    // receive
    expected.add_packet(ack);

    compare_traces(expected);
}

TEST_F(BackEndMockTestDrop22, sendReceiveTestPassiveToActiveDrop22) {
    drop_ack_send_data_passive_to_active();
}

void drop_first_data_packet_passive_to_active() {
    string data = random_string(1);
    send_receive_test(data);

    NetworkTrace expected;

    // Send
    expected.add_packet(get_syn());
    // receive
    expected.add_packet(get_syn());

    // send
    expected.add_packet(get_synack());
    // receive
    expected.add_packet(get_synack());

    // send
    expected.add_packet(get_ack());
    // receive
    expected.add_packet(get_ack());


    TCPPacket* data_packet = get_base_tcp_packet();
    data_packet->set_data((unsigned char*) data.c_str(), data.size());
    data_packet->set_tcp_sequence_number(2);
    data_packet->set_tcp_ack_number(3);
    data_packet->set_source_port(5002);
    data_packet->set_destination_port(1000);
    data_packet->set_tcp_ack(true);

    // send (drop)
    expected.add_packet(data_packet);
    // resend
    expected.add_packet(data_packet);
    // receive
    expected.add_packet(data_packet);

    TCPPacket* ack = get_base_tcp_packet();
    ack->set_tcp_sequence_number(3);
    ack->set_tcp_ack_number(3);
    ack->set_source_port(1000);
    ack->set_destination_port(5002);
    ack->set_tcp_ack(true);

    // send
    expected.add_packet(ack);
    // receive
    expected.add_packet(ack);

    compare_traces(expected);
}

TEST_F(BackEndMockTestDrop23, sendReceiveTestPassiveToActiveDrop23) {
    drop_first_data_packet_passive_to_active();
}

void drop_first_data_ack_packet_passive_to_active() {
    string data = random_string(1);
    send_receive_test(data);

    NetworkTrace expected;

    // Send
    expected.add_packet(get_syn());
    // receive
    expected.add_packet(get_syn());

    // send
    expected.add_packet(get_synack());
    // receive
    expected.add_packet(get_synack());

    // send
    expected.add_packet(get_ack());
    // receive
    expected.add_packet(get_ack());


    TCPPacket* data_packet = get_base_tcp_packet();
    data_packet->set_data((unsigned char*) data.c_str(), data.size());
    data_packet->set_tcp_sequence_number(2);
    data_packet->set_tcp_ack_number(3);
    data_packet->set_source_port(5002);
    data_packet->set_destination_port(1000);
    data_packet->set_tcp_ack(true);

    // send
    expected.add_packet(data_packet);
    // receive
    expected.add_packet(data_packet);

    TCPPacket* ack = get_base_tcp_packet();
    ack->set_tcp_sequence_number(3);
    ack->set_tcp_ack_number(3);
    ack->set_source_port(1000);
    ack->set_destination_port(5002);
    ack->set_tcp_ack(true);

    // send (drop)
    expected.add_packet(ack);

    // resend
    expected.add_packet(data_packet);
    // receive
    expected.add_packet(data_packet);

    // send
    expected.add_packet(ack);
    // receive
    expected.add_packet(ack);

    compare_traces(expected);
}

TEST_F(BackEndMockTestDrop33, sendReceiveTestPassiveToActiveDrop33) {
    drop_first_data_ack_packet_passive_to_active();
}

TEST_F(BackEndMockTestDropRandom10Percent, sendReceiveTestPassiveToActiveDropRandom) {
    send_receive_test(random_string(20000));
}

TEST_F(BackEndMockTestDropRandom20Percent, sendReceiveTestPassiveToActiveDropRandom) {
    send_receive_test(random_string(20000));
}

TEST_F(BackEndMockTestDropRandom30Percent, sendReceiveTestPassiveToActiveDropRandom) {
    send_receive_test(random_string(20000));
}

TEST_F(BackEndMockTestDropRandom40Percent, sendReceiveTestPassiveToActiveDropRandom) {
    send_receive_test(random_string(20000));
}

TEST_F(BackEndMockTestDropRandom50Percent, sendReceiveTestPassiveToActiveDropRandom) {
    send_receive_test(random_string(20000));
}

TEST_F(BackEndMockTestDropRandom60Percent, sendReceiveTestPassiveToActiveDropRandom) {
    send_receive_test(random_string(20000));
}