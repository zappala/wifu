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

#include "../headers/PacketLogReader.h"
#include "../headers/packet/TCPPacket.h"

void* thread(void* args) {

    struct var* v = (struct var*) args;
    AddressPort* to_bind = v->to_bind_;
    Semaphore* sem = v->sem_;

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

}

void connect_test() {
    AddressPort to_connect("127.0.0.1", 5002);

    pthread_t t;
    struct var v;
    Timer timer;
    int client;
    int result;


    v.sem_ = new Semaphore();
    v.sem_->init(0);
    v.to_bind_ = new AddressPort("127.0.0.1", 5002);


    if (pthread_create(&t, NULL, &thread, &v) != 0)
        FAIL() << "Error creating new thread in IntegrationTest.connectTest";

    v.sem_->wait();

    // Make sure that the thread is in the accept state
    usleep(50000);

    // Create client

    timer.start();
    client = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);
    result = wifu_connect(client, (const struct sockaddr *) to_connect.get_network_struct_ptr(), sizeof (struct sockaddr_in));
    timer.stop();
    ASSERT_EQ(0, result);

    cout << "Duration (us) to create a socket and connect on localhost via wifu: " << timer.get_duration_microseconds() << endl;
    sleep(5);
}

TEST_F(BackEndTest, connectTest) {
    connect_test();
}

void compare_traces(NetworkTrace& expected) {
    PacketLogReader reader(LOG_FILENAME);
    NetworkTrace* actual = reader.get_trace();

    ASSERT_EQ(expected, *actual) << expected.get_packet_trace(*actual);
}

TCPPacket* get_syn() {
    TCPPacket* syn = new TCPPacket();
    syn->set_ip_protocol(SIMPLE_TCP);
    syn->set_ip_source_address_s("127.0.0.1");
    syn->set_ip_destination_address_s("127.0.0.1");
    syn->set_destination_port(5002);
    syn->set_source_port(1000);
    syn->set_tcp_sequence_number(1);
    syn->set_tcp_ack_number(0);
    syn->set_tcp_syn(true);
    return syn;
}

TCPPacket* get_synack() {
    TCPPacket* synack = new TCPPacket();
    synack->set_ip_protocol(SIMPLE_TCP);
    synack->set_ip_source_address_s("127.0.0.1");
    synack->set_ip_destination_address_s("127.0.0.1");
    synack->set_destination_port(1000);
    synack->set_source_port(5002);
    synack->set_tcp_sequence_number(1);
    synack->set_tcp_ack_number(2);
    synack->set_tcp_syn(true);
    synack->set_tcp_ack(true);
    return synack;
}

TCPPacket* get_ack() {
    TCPPacket* ack = new TCPPacket();
    ack->set_ip_protocol(SIMPLE_TCP);
    ack->set_ip_source_address_s("127.0.0.1");
    ack->set_ip_destination_address_s("127.0.0.1");
    ack->set_destination_port(5002);
    ack->set_source_port(1000);
    ack->set_tcp_sequence_number(2);
    ack->set_tcp_ack_number(2);
    ack->set_tcp_ack(true);
    return ack;
}


void drop_none() {
    connect_test();
    NetworkTrace expected;

    // send
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

    compare_traces(expected);
}

TEST_F(BackEndMockTestDropNone, mockConnectTest) {
    drop_none();
}

void drop_syn() {
    connect_test();

    NetworkTrace expected;

    // Send and drop
    expected.add_packet(get_syn());
    // resend
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

    compare_traces(expected);
}

// Drop packets

TEST_F(BackEndMockTestDrop10, mockConnectTest10) {
    drop_syn();
}