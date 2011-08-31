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

#include "../headers/BackEndTest.h"

#include "../headers/PacketLogReader.h"
#include "../headers/packet/TCPPacket.h"

#include "../headers/PacketTraceHelper.h"

void* thread(void* args) {

    struct var* v = (struct var*) args;
    AddressPort* to_bind = v->to_bind_;
    Semaphore* sem = v->sem_;

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

    AddressPort ap(&addr);
    gcstring address("127.0.0.1");
    gcstring res = ap.get_address();
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
    client = wifu_socket(AF_INET, SOCK_STREAM, TCP_TAHOE);
    result = wifu_connect(client, (const struct sockaddr *) to_connect.get_network_struct_ptr(), sizeof (struct sockaddr_in));
    timer.stop();
    ASSERT_EQ(0, result);

    cout << "Duration (us) to create a socket and connect on localhost via wifu: " << timer.get_duration_microseconds() << endl;
    sleep(5);
}

TEST_F(BackEndTest, connectTest) {
    connect_test();
}



void drop_none() {
    connect_test();
    NetworkTrace expected;

    // send
    expected.add_packet(get_syn(TCP_TAHOE));
    // receive
    expected.add_packet(get_syn(TCP_TAHOE));


    // send
    expected.add_packet(get_synack(TCP_TAHOE));
    // receive
    expected.add_packet(get_synack(TCP_TAHOE));


    // send
    expected.add_packet(get_ack(TCP_TAHOE));
    // receive
    expected.add_packet(get_ack(TCP_TAHOE));

    compare_traces(expected);
}

TEST_F(BackEndMockTestDropNone, mockConnectTest) {
    drop_none();
}

void drop_syn() {
    connect_test();

    NetworkTrace expected;

    // Send and drop
    expected.add_packet(get_syn(TCP_TAHOE));
    // resend
    expected.add_packet(get_syn(TCP_TAHOE));
    // receive
    expected.add_packet(get_syn(TCP_TAHOE));


    // send
    expected.add_packet(get_synack(TCP_TAHOE));
    // receive
    expected.add_packet(get_synack(TCP_TAHOE));


    // send
    expected.add_packet(get_ack(TCP_TAHOE));
    // receive
    expected.add_packet(get_ack(TCP_TAHOE));

    compare_traces(expected);
}

// Drop packets

TEST_F(BackEndMockTestDrop10, mockConnectTest10) {
    drop_syn();
}

void drop_synack12_delay_synack12() {
    connect_test();

    NetworkTrace expected;

    // Send
    expected.add_packet(get_syn(TCP_TAHOE));
    // receive
    expected.add_packet(get_syn(TCP_TAHOE));

    // send (drop)
    expected.add_packet(get_synack(TCP_TAHOE));

    // resend
    expected.add_packet(get_syn(TCP_TAHOE));
    // recieve
    expected.add_packet(get_syn(TCP_TAHOE));

    // resend
    expected.add_packet(get_synack(TCP_TAHOE));
    // receive
    expected.add_packet(get_synack(TCP_TAHOE));
    
    // send
    expected.add_packet(get_ack(TCP_TAHOE));
    // receive
    expected.add_packet(get_ack(TCP_TAHOE));

    compare_traces(expected);
}

TEST_F(BackEndMockTestDrop12Delay12, mockConnectTestDrop12Delay12) {
    drop_synack12_delay_synack12();
}

void drop_synack12_delay_syn10() {
    connect_test();

    NetworkTrace expected;

    // Send
    expected.add_packet(get_syn(TCP_TAHOE));
    // receive
    expected.add_packet(get_syn(TCP_TAHOE));

    // send (drop)
    expected.add_packet(get_synack(TCP_TAHOE));

    // resend (delayed)
    expected.add_packet(get_syn(TCP_TAHOE));

    // resend
    expected.add_packet(get_synack(TCP_TAHOE));
    // receive
    expected.add_packet(get_synack(TCP_TAHOE));
    
    // send
    expected.add_packet(get_ack(TCP_TAHOE));
    // receive
    expected.add_packet(get_ack(TCP_TAHOE));

    // receive
    expected.add_packet(get_syn(TCP_TAHOE));

    // the syn elicits and ACK because it is now invalid

    TCPPacket* p = get_synack(TCP_TAHOE);
    p->set_tcp_sequence_number(2);
    p->set_tcp_syn(false);
    // send
    expected.add_packet(p);
    // receive
    expected.add_packet(p);


    compare_traces(expected);
}

TEST_F(BackEndMockTestDrop12Delay10, mockConnectTestDrop12Delay10) {
    drop_synack12_delay_syn10();
}

void drop_ack() {
    connect_test();

    NetworkTrace expected;

    // Send
    expected.add_packet(get_syn(TCP_TAHOE));
    // receive
    expected.add_packet(get_syn(TCP_TAHOE));

    // send
    expected.add_packet(get_synack(TCP_TAHOE));
    // receive
    expected.add_packet(get_synack(TCP_TAHOE));

    // send
    expected.add_packet(get_ack(TCP_TAHOE));

    // resend
    expected.add_packet(get_synack(TCP_TAHOE));
    // receive
    expected.add_packet(get_synack(TCP_TAHOE));

    // send
    expected.add_packet(get_ack(TCP_TAHOE));
    // receive
    expected.add_packet(get_ack(TCP_TAHOE));


    compare_traces(expected);
}

TEST_F(BackEndMockTestDrop22, mockConnectTest22) {
    drop_ack();
}