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
#include "../headers/packet/TCPPacket.h"

#include "../headers/BackEndTest.h"

#include "../headers/PacketTraceHelper.h"
#include "Utils.h"

void* active_to_passive_thread(void* args) {

    struct var* v = (struct var*) args;
    AddressPort* to_bind = v->to_bind_;
    Semaphore* sem = v->sem_;

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
    if ((connection = wifu_accept(server, (struct sockaddr *) & addr, &length)) < 0) {
        // TODO: we need to check errors and make sure they happen when they should
        ADD_FAILURE() << "Problem in Accept";
    }

    AddressPort ap(&addr);
    gcstring address("127.0.0.1");
    //TODO: move this into real network test
//    gcstring address("192.168.21.103");
    gcstring res = ap.get_address();
    EXPECT_EQ(address, res);
    //    cout << "Connected to: " << ap.to_s() << endl;

    // TODO: Check the results of wifu_accept, probably need to wait for send, recv to be implemented

    int size = 50000;
    char buffer[size];
    memset(buffer, 0, size);
    gcstring all_received = "";

    for (int count = 0; count < expected.length(); ++count) {

        gcstring exp = expected.substr(count, 1);
        int return_value = wifu_recv(connection, &buffer, 1, 0);
        gcstring actual(buffer);
        all_received.append(actual);


        EXPECT_EQ(1, return_value);
        EXPECT_EQ(exp, actual);

    }
    EXPECT_EQ(expected, all_received);
    //    cout << "Received: " << all_received << endl;
    sem->post();
}

/**
 * @param num_bytes The number of bytes to send, currently, this is also the number of packets to send (we sent one data byte per packet)
 *
 */
void active_to_passive_test(gcstring message) {
    AddressPort to_connect("127.0.0.1", 5002);
    //use below for over-the-network tests; TODO: break out into separate test function
//    AddressPort to_connect("192.168.21.103", 5002);

    pthread_t t;
    struct var v;
    Timer timer;
    int client;
    int result;

    v.sem_ = new Semaphore();
    v.sem_->init(0);
    v.to_bind_ = new AddressPort("127.0.0.1", 5002);
//    v.to_bind_ = new AddressPort("192.168.21.103", 5002);

    //Specify the number of bytes to send here.
    v.expected_string = message;


    if (pthread_create(&(t), NULL, &active_to_passive_thread, &(v)) != 0) {
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
    ASSERT_EQ(0, result);

    //    cout << "Duration (us) to create a socket and connect on localhost via wifu: " << timer.get_duration_microseconds() << endl;

    int size = 50000;
    char buffer[size];

    memcpy(buffer, message.c_str(), message.length());

    int count = 1;
    int num_sent = 0;

    // TODO: this only sends one character at a time
    for (int i = 0; i < message.length(); i++) {
        num_sent += wifu_send(client, &(buffer[i]), count, 0);
    }

    EXPECT_EQ(message.length(), num_sent);

    //    cout << "Sent: " << message << endl;

    struct timespec ts;
    // wait max of 30 mins
    Utils::get_timespec_future_time(60 * 30, 0, &ts);
    v.sem_->timed_wait(&ts);
}

TEST_F(BackEndMockTestDropNone, sendReceiveTestActiveToPassive1) {
    active_to_passive_test(random_string(1));
}

TEST_F(BackEndMockTestDropNone, sendReceiveTestActiveToPassive10) {
    active_to_passive_test(random_string(10));
}

TEST_F(BackEndMockTestDropNone, sendReceiveTestActiveToPassive100) {
    active_to_passive_test(random_string(100));
}

TEST_F(BackEndMockTestDropNone, sendReceiveTestActiveToPassive1000) {
    active_to_passive_test(random_string(1000));
}

TEST_F(BackEndMockTestDropNone, sendReceiveTestActiveToPassive2000) {
    active_to_passive_test(random_string(2000));
}

TEST_F(BackEndMockTestDropNone, sendReceiveTestActiveToPassive5000) {
    active_to_passive_test(random_string(5000));
}

TEST_F(BackEndMockTestDropNone, sendReceiveTestActiveToPassive50000) {
    active_to_passive_test(random_string(50000));
}

TEST_F(BackEndTest, sendReceiveTestActiveToPassiveReal) {
    active_to_passive_test(random_string(50000));
}

void drop_ack_send_data() {
    gcstring data = random_string(1);
    active_to_passive_test(data);

    NetworkTrace expected;

    // Send
    expected.add_packet(get_syn(SIMPLE_TCP));
    // receive
    expected.add_packet(get_syn(SIMPLE_TCP));

    // send
    expected.add_packet(get_synack(SIMPLE_TCP));
    // receive
    expected.add_packet(get_synack(SIMPLE_TCP));

    // send
    expected.add_packet(get_ack(SIMPLE_TCP));

    TCPPacket* data_packet = get_base_tcp_packet(SIMPLE_TCP);
    data_packet->set_tcp_sequence_number(3);
    data_packet->set_tcp_ack_number(2);
    data_packet->set_destination_port(5002);
    data_packet->set_source_port(1000);
    data_packet->set_tcp_ack(true);
    data_packet->insert_tcp_header_option(new TCPTimestampOption());
    data_packet->set_data((unsigned char*) data.c_str(), data.size());

    // send
    expected.add_packet(data_packet);
    // receive
    expected.add_packet(data_packet);

    TCPPacket* ack = get_base_tcp_packet_ts(SIMPLE_TCP);
    ack->set_tcp_sequence_number(2);
    ack->set_tcp_ack_number(4);
    ack->set_source_port(5002);
    ack->set_destination_port(1000);
    ack->set_tcp_ack(true);

    // send
    expected.add_packet(ack);
    // receive
    expected.add_packet(ack);



    compare_traces(expected);
}

TEST_F(BackEndMockTestDrop22, sendReceiveTestActiveToPassive22) {
    drop_ack_send_data();
}

void drop_ack_and_data() {
    gcstring data = random_string(1);
    active_to_passive_test(data);

    NetworkTrace expected;

    // Send
    expected.add_packet(get_syn(SIMPLE_TCP));
    // receive
    expected.add_packet(get_syn(SIMPLE_TCP));

    // send
    expected.add_packet(get_synack(SIMPLE_TCP));
    // receive
    expected.add_packet(get_synack(SIMPLE_TCP));

    // send
    expected.add_packet(get_ack(SIMPLE_TCP));

    TCPPacket* data_packet = get_base_tcp_packet(SIMPLE_TCP);
    data_packet->set_tcp_sequence_number(3);
    data_packet->set_tcp_ack_number(2);
    data_packet->set_destination_port(5002);
    data_packet->set_source_port(1000);
    data_packet->set_tcp_ack(true);
    data_packet->insert_tcp_header_option(new TCPTimestampOption());
    data_packet->set_data((unsigned char*) data.c_str(), data.size());

    // send
    expected.add_packet(data_packet);


    // resend
    expected.add_packet(get_synack(SIMPLE_TCP));
    // receive (it gets ignored)
    expected.add_packet(get_synack(SIMPLE_TCP));

    // resend
    expected.add_packet(data_packet);
    // receive
    expected.add_packet(data_packet);

    TCPPacket* ack = get_base_tcp_packet_ts(SIMPLE_TCP);
    ack->set_tcp_sequence_number(2);
    ack->set_tcp_ack_number(4);
    ack->set_source_port(5002);
    ack->set_destination_port(1000);
    ack->set_tcp_ack(true);

    // send
    expected.add_packet(ack);
    // receive
    expected.add_packet(ack);

    compare_traces(expected);
}

TEST_F(BackEndMockTestDrop22Drop32, sendReceiveTestActiveToPassive22and32) {
    drop_ack_and_data();
}

void drop_first_data_packet() {
    gcstring data = random_string(1);
    active_to_passive_test(data);
    sleep(2);

    NetworkTrace expected;

    // Send
    expected.add_packet(get_syn(SIMPLE_TCP));
    // receive
    expected.add_packet(get_syn(SIMPLE_TCP));

    // send
    expected.add_packet(get_synack(SIMPLE_TCP));
    // receive
    expected.add_packet(get_synack(SIMPLE_TCP));

    // send
    expected.add_packet(get_ack(SIMPLE_TCP));
    // receive
    expected.add_packet(get_ack(SIMPLE_TCP));


    TCPPacket* data_packet = get_base_tcp_packet(SIMPLE_TCP);
    data_packet->set_tcp_sequence_number(3);
    data_packet->set_tcp_ack_number(2);
    data_packet->set_destination_port(5002);
    data_packet->set_source_port(1000);
    data_packet->set_tcp_ack(true);
    data_packet->insert_tcp_header_option(new TCPTimestampOption());
    data_packet->set_data((unsigned char*) data.c_str(), data.size());

    // send
    expected.add_packet(data_packet);
    // resend
    expected.add_packet(data_packet);
    // receive
    expected.add_packet(data_packet);

    TCPPacket* ack = get_base_tcp_packet_ts(SIMPLE_TCP);
    ack->set_tcp_sequence_number(2);
    ack->set_tcp_ack_number(4);
    ack->set_source_port(5002);
    ack->set_destination_port(1000);
    ack->set_tcp_ack(true);

    // send
    expected.add_packet(ack);
    // receive
    expected.add_packet(ack);

    compare_traces(expected);
}

TEST_F(BackEndMockTestDrop32, sendReceiveTestActiveToPassive32) {
    drop_first_data_packet();
}

void drop_first_data_ack() {
    gcstring data = random_string(1);
    active_to_passive_test(data);
    sleep(2);

    NetworkTrace expected;

    // Send
    expected.add_packet(get_syn(SIMPLE_TCP));
    // receive
    expected.add_packet(get_syn(SIMPLE_TCP));

    // send
    expected.add_packet(get_synack(SIMPLE_TCP));
    // receive
    expected.add_packet(get_synack(SIMPLE_TCP));

    // send
    expected.add_packet(get_ack(SIMPLE_TCP));
    // receive
    expected.add_packet(get_ack(SIMPLE_TCP));

    // connected

    TCPPacket* data_packet = get_base_tcp_packet(SIMPLE_TCP);
    data_packet->set_tcp_sequence_number(3);
    data_packet->set_tcp_ack_number(2);
    data_packet->set_destination_port(5002);
    data_packet->set_source_port(1000);
    data_packet->set_tcp_ack(true);
    data_packet->insert_tcp_header_option(new TCPTimestampOption());
    data_packet->set_data((unsigned char*) data.c_str(), data.size());

    // send
    expected.add_packet(data_packet);
    // receive
    expected.add_packet(data_packet);

    TCPPacket* ack = get_base_tcp_packet_ts(SIMPLE_TCP);
    ack->set_tcp_sequence_number(2);
    ack->set_tcp_ack_number(4);
    ack->set_source_port(5002);
    ack->set_destination_port(1000);
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

TEST_F(BackEndMockTestDrop24, sendReceiveTestActiveToPassive24) {
    drop_first_data_ack();
}

TEST_F(BackEndMockTestDropRandom10Percent, sendReceiveTestActiveToPassiveDropRandom) {
    active_to_passive_test(random_string(20000));
}

TEST_F(BackEndMockTestDropRandom20Percent, sendReceiveTestActiveToPassiveDropRandom) {
    active_to_passive_test(random_string(20000));
}

TEST_F(BackEndMockTestDropRandom30Percent, sendReceiveTestActiveToPassiveDropRandom) {
    active_to_passive_test(random_string(20000));
}

TEST_F(BackEndMockTestDropRandom40Percent, sendReceiveTestActiveToPassiveDropRandom) {
    active_to_passive_test(random_string(20000));
}

TEST_F(BackEndMockTestDropRandom50Percent, sendReceiveTestActiveToPassiveDropRandom) {
    active_to_passive_test(random_string(20000));
}

TEST_F(BackEndMockTestDropRandom60Percent, sendReceiveTestActiveToPassiveDropRandom) {
    active_to_passive_test(random_string(20000));
}