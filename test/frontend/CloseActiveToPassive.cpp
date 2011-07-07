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

void* close_active_to_passive_thread(void* args) {

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
    if ((connection = wifu_accept(server, (struct sockaddr *) & addr, &length)) < 0) {
        // TODO: we need to check errors and make sure they happen when they should
        ADD_FAILURE() << "Problem in Accept";
    }

    flag->post();

    AddressPort ap(&addr);
    gcstring address("127.0.0.1");
    gcstring res = ap.get_address();
    EXPECT_EQ(address, res);
    //    cout << "Connected to: " << ap.to_s() << endl;

    // TODO: Check the results of wifu_accept, probably need to wait for send, recv to be implemented

    int size = 50000;
    char buffer[size];
    memset(buffer, 0, size);
    gcstring all_received = "";

    while (true) {
        int return_value = wifu_recv(connection, &buffer, 1, 0);

        if (return_value == 0) {
            //            cout << "Close Thread BREAK" << endl;
            break;
        }

        //        cout << "Received: \"" << buffer << "\"" << endl;
        //        cout << "Num receive: " << return_value << endl;
        EXPECT_EQ(1, return_value);

        gcstring actual(buffer);
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
void close_active_to_passive_test(gcstring message) {
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


    if (pthread_create(&(t), NULL, &close_active_to_passive_thread, &(v)) != 0) {
        FAIL() << "Error creating new thread in IntegrationTest.h";
    }

    v.sem_->wait();

    // Create client

    timer.start();
    client = wifu_socket(AF_INET, SOCK_STREAM, SIMPLE_TCP);
    result = wifu_connect(client, (const struct sockaddr *) to_connect.get_network_struct_ptr(), sizeof (struct sockaddr_in));
    timer.stop();
    ASSERT_EQ(0, result);

    v.flag_->wait();

//        cout << "Duration (us) to create a socket and connect on localhost via wifu: " << timer.get_duration_microseconds() << endl;

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

//        cout << "Sent: " << message << endl;
    wifu_close(client);

    // TODO: test send/receive on the socket to ensure we aren't allowed to do anythin after close

    v.done_->wait();
    sleep(2);
}

void close_active_to_passive_drop_none() {
    // <editor-fold defaultstate="collapsed" desc="setup">
    gcstring data = random_string(1);
    close_active_to_passive_test(data);

    NetworkTrace expected;

    // Connect

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

    // Data
    TCPPacket* data_packet = get_base_tcp_packet(SIMPLE_TCP);
    data_packet->set_destination_port(5002);
    data_packet->set_source_port(1000);
    data_packet->set_tcp_ack(true);
    data_packet->set_tcp_sequence_number(3);
    data_packet->set_tcp_ack_number(2);
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
    expected.add_packet(ack); // </editor-fold>

    // Close
    // Active to Passive
    TCPPacket* fin1 = get_base_tcp_packet_ts(SIMPLE_TCP);
    fin1->set_tcp_sequence_number(4);
    fin1->set_tcp_ack_number(3);
    fin1->set_source_port(1000);
    fin1->set_destination_port(5002);
    fin1->set_tcp_fin(true);
    fin1->set_tcp_ack(true);

    // send
    expected.add_packet(fin1);
    // receive
    expected.add_packet(fin1);

    TCPPacket* ack1 = get_base_tcp_packet_ts(SIMPLE_TCP);
    ack1->set_tcp_sequence_number(3);
    ack1->set_tcp_ack_number(5);
    ack1->set_source_port(5002);
    ack1->set_destination_port(1000);
    ack1->set_tcp_ack(true);

    // send
    expected.add_packet(ack1);
    // receive
    expected.add_packet(ack1);

    // Close
    // Passive to Active
    TCPPacket* fin2 = get_base_tcp_packet_ts(SIMPLE_TCP);
    fin2->set_tcp_sequence_number(4);
    fin2->set_tcp_ack_number(5);
    fin2->set_source_port(5002);
    fin2->set_destination_port(1000);
    fin2->set_tcp_fin(true);
    fin2->set_tcp_ack(true);

    // send
    expected.add_packet(fin2);
    // receive
    expected.add_packet(fin2);


    TCPPacket* ack2 = get_base_tcp_packet_ts(SIMPLE_TCP);
    ack2->set_tcp_sequence_number(5);
    ack2->set_tcp_ack_number(5);
    ack2->set_source_port(1000);
    ack2->set_destination_port(5002);
    ack2->set_tcp_ack(true);

    // send
    expected.add_packet(ack2);
    // receive
    expected.add_packet(ack2);

    compare_traces(expected);
}

TEST_F(BackEndMockTestDropNone, closeTestActiveToPassive) {
    close_active_to_passive_drop_none();
}

void close_active_to_passive_drop_first_fin() {
    // <editor-fold defaultstate="collapsed" desc="setup">
    gcstring data = random_string(1);
    close_active_to_passive_test(data);

    NetworkTrace expected;

    // Connect

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

    // Data

    TCPPacket* data_packet = get_base_tcp_packet(SIMPLE_TCP);
    data_packet->set_destination_port(5002);
    data_packet->set_source_port(1000);
    data_packet->set_tcp_ack(true);
    data_packet->set_tcp_sequence_number(3);
    data_packet->set_tcp_ack_number(2);
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
    expected.add_packet(ack); // </editor-fold>

    // Close
    // Active to Passive
    TCPPacket* fin1 = get_base_tcp_packet_ts(SIMPLE_TCP);
    fin1->set_tcp_sequence_number(4);
    fin1->set_tcp_ack_number(3);
    fin1->set_source_port(1000);
    fin1->set_destination_port(5002);
    fin1->set_tcp_fin(true);
    fin1->set_tcp_ack(true);

    // send (drop)
    expected.add_packet(fin1);
    // resend
    expected.add_packet(fin1);
    // receive
    expected.add_packet(fin1);

    TCPPacket* ack1 = get_base_tcp_packet_ts(SIMPLE_TCP);
    ack1->set_tcp_sequence_number(3);
    ack1->set_tcp_ack_number(5);
    ack1->set_source_port(5002);
    ack1->set_destination_port(1000);
    ack1->set_tcp_ack(true);

    // send
    expected.add_packet(ack1);
    // receive
    expected.add_packet(ack1);

    // Close
    // Passive to Active
    TCPPacket* fin2 = get_base_tcp_packet_ts(SIMPLE_TCP);
    fin2->set_tcp_sequence_number(4);
    fin2->set_tcp_ack_number(5);
    fin2->set_source_port(5002);
    fin2->set_destination_port(1000);
    fin2->set_tcp_fin(true);
    fin2->set_tcp_ack(true);

    // send
    expected.add_packet(fin2);
    // receive
    expected.add_packet(fin2);


    TCPPacket* ack2 = get_base_tcp_packet_ts(SIMPLE_TCP);
    ack2->set_tcp_sequence_number(5);
    ack2->set_tcp_ack_number(5);
    ack2->set_source_port(1000);
    ack2->set_destination_port(5002);
    ack2->set_tcp_ack(true);

    // send
    expected.add_packet(ack2);
    // receive
    expected.add_packet(ack2);

    compare_traces(expected);
}

TEST_F(BackEndMockTestDrop43, closeTestActiveToPassiveDrop43) {
    close_active_to_passive_drop_first_fin();
}

void close_active_to_passive_drop_first_ack() {
    // <editor-fold defaultstate="collapsed" desc="setup">
    gcstring data = random_string(1);
    close_active_to_passive_test(data);

    NetworkTrace expected;

    // Connect

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

    // Data

    TCPPacket* data_packet = get_base_tcp_packet(SIMPLE_TCP);
    data_packet->set_destination_port(5002);
    data_packet->set_source_port(1000);
    data_packet->set_tcp_ack(true);
    data_packet->set_tcp_sequence_number(3);
    data_packet->set_tcp_ack_number(2);
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
    expected.add_packet(ack); // </editor-fold>

    // Close
    // Active to Passive
    TCPPacket* fin1 = get_base_tcp_packet_ts(SIMPLE_TCP);
    fin1->set_tcp_sequence_number(4);
    fin1->set_tcp_ack_number(3);
    fin1->set_source_port(1000);
    fin1->set_destination_port(5002);
    fin1->set_tcp_fin(true);
    fin1->set_tcp_ack(true);

    // send
    expected.add_packet(fin1);
    // receive
    expected.add_packet(fin1);

    TCPPacket* ack1 = get_base_tcp_packet_ts(SIMPLE_TCP);
    ack1->set_tcp_sequence_number(3);
    ack1->set_tcp_ack_number(5);
    ack1->set_source_port(5002);
    ack1->set_destination_port(1000);
    ack1->set_tcp_ack(true);

    // send (drop)
    expected.add_packet(ack1);

    // Close
    // Passive to Active
    TCPPacket* fin2 = get_base_tcp_packet_ts(SIMPLE_TCP);
    fin2->set_tcp_sequence_number(4);
    fin2->set_tcp_ack_number(5);
    fin2->set_source_port(5002);
    fin2->set_destination_port(1000);
    fin2->set_tcp_fin(true);
    fin2->set_tcp_ack(true);

    // send
    expected.add_packet(fin2);
    // receive
    expected.add_packet(fin2);


    TCPPacket* ack2 = get_base_tcp_packet_ts(SIMPLE_TCP);
    ack2->set_tcp_sequence_number(5);
    ack2->set_tcp_ack_number(5);
    ack2->set_source_port(1000);
    ack2->set_destination_port(5002);
    ack2->set_tcp_ack(true);

    // send
    expected.add_packet(ack2);
    // receive
    expected.add_packet(ack2);

    compare_traces(expected);
}

TEST_F(BackEndMockTestDrop35, closeTestActiveToPassiveDrop35) {
    close_active_to_passive_drop_first_ack();
}

void close_active_to_passive_drop_second_fin() {
    // <editor-fold defaultstate="collapsed" desc="setup">
    gcstring data = random_string(1);
    close_active_to_passive_test(data);

    NetworkTrace expected;

    // Connect

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

    // Data
    TCPPacket* data_packet = get_base_tcp_packet(SIMPLE_TCP);
    data_packet->set_destination_port(5002);
    data_packet->set_source_port(1000);
    data_packet->set_tcp_ack(true);
    data_packet->set_tcp_sequence_number(3);
    data_packet->set_tcp_ack_number(2);
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
    expected.add_packet(ack); // </editor-fold>

    // Close
    // Active to Passive
    TCPPacket* fin1 = get_base_tcp_packet_ts(SIMPLE_TCP);
    fin1->set_tcp_sequence_number(4);
    fin1->set_tcp_ack_number(3);
    fin1->set_source_port(1000);
    fin1->set_destination_port(5002);
    fin1->set_tcp_fin(true);
    fin1->set_tcp_ack(true);

    // send
    expected.add_packet(fin1);
    // receive
    expected.add_packet(fin1);

    TCPPacket* ack1 = get_base_tcp_packet_ts(SIMPLE_TCP);
    ack1->set_tcp_sequence_number(3);
    ack1->set_tcp_ack_number(5);
    ack1->set_source_port(5002);
    ack1->set_destination_port(1000);
    ack1->set_tcp_ack(true);

    // send
    expected.add_packet(ack1);
    // receive
    expected.add_packet(ack1);

    // Close
    // Passive to Active
    TCPPacket* fin2 = get_base_tcp_packet_ts(SIMPLE_TCP);
    fin2->set_tcp_sequence_number(4);
    fin2->set_tcp_ack_number(5);
    fin2->set_source_port(5002);
    fin2->set_destination_port(1000);
    fin2->set_tcp_fin(true);
    fin2->set_tcp_ack(true);

    // send (drop)
    expected.add_packet(fin2);
    // resend
    expected.add_packet(fin2);
    // receive
    expected.add_packet(fin2);

    TCPPacket* ack2 = get_base_tcp_packet_ts(SIMPLE_TCP);
    ack2->set_tcp_sequence_number(5);
    ack2->set_tcp_ack_number(5);
    ack2->set_source_port(1000);
    ack2->set_destination_port(5002);
    ack2->set_tcp_ack(true);

    // send
    expected.add_packet(ack2);
    // receive
    expected.add_packet(ack2);

    compare_traces(expected);
}

TEST_F(BackEndMockTestDrop45, closeTestActiveToPassiveDrop45) {
    close_active_to_passive_drop_second_fin();
}

void close_active_to_passive_drop_second_ack() {
    // <editor-fold defaultstate="collapsed" desc="setup">
    gcstring data = random_string(1);
    close_active_to_passive_test(data);

    NetworkTrace expected;

    // Connect

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

    // Data
    TCPPacket* data_packet = get_base_tcp_packet(SIMPLE_TCP);
    data_packet->set_destination_port(5002);
    data_packet->set_source_port(1000);
    data_packet->set_tcp_ack(true);
    data_packet->set_tcp_sequence_number(3);
    data_packet->set_tcp_ack_number(2);
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
    expected.add_packet(ack); // </editor-fold>

    // Close
    // Active to Passive
    TCPPacket* fin1 = get_base_tcp_packet_ts(SIMPLE_TCP);
    fin1->set_tcp_sequence_number(4);
    fin1->set_tcp_ack_number(3);
    fin1->set_source_port(1000);
    fin1->set_destination_port(5002);
    fin1->set_tcp_fin(true);
    fin1->set_tcp_ack(true);

    // send
    expected.add_packet(fin1);
    // receive
    expected.add_packet(fin1);

    TCPPacket* ack1 = get_base_tcp_packet_ts(SIMPLE_TCP);
    ack1->set_tcp_sequence_number(3);
    ack1->set_tcp_ack_number(5);
    ack1->set_source_port(5002);
    ack1->set_destination_port(1000);
    ack1->set_tcp_ack(true);

    // send
    expected.add_packet(ack1);
    // receive
    expected.add_packet(ack1);

    // Close
    // Passive to Active
    TCPPacket* fin2 = get_base_tcp_packet_ts(SIMPLE_TCP);
    fin2->set_tcp_sequence_number(4);
    fin2->set_tcp_ack_number(5);
    fin2->set_source_port(5002);
    fin2->set_destination_port(1000);
    fin2->set_tcp_fin(true);
    fin2->set_tcp_ack(true);

    // send
    expected.add_packet(fin2);
    // receive
    expected.add_packet(fin2);

    TCPPacket* ack2 = get_base_tcp_packet_ts(SIMPLE_TCP);
    ack2->set_tcp_sequence_number(5);
    ack2->set_tcp_ack_number(5);
    ack2->set_source_port(1000);
    ack2->set_destination_port(5002);
    ack2->set_tcp_ack(true);

    // send
    expected.add_packet(ack2);


    // resend
    expected.add_packet(fin2);
    // receive
    expected.add_packet(fin2);

    // resend
    expected.add_packet(ack2);
    // receive
    expected.add_packet(ack2);


    compare_traces(expected);
}

TEST_F(BackEndMockTestDrop55, closeTestActiveToPassiveDrop55) {
    close_active_to_passive_drop_second_ack();
}

void close_active_to_passive_drop_first_ack_and_second_fin() {
    // <editor-fold defaultstate="collapsed" desc="setup">
    gcstring data = random_string(1);
    close_active_to_passive_test(data);

    NetworkTrace expected;

    // Connect

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

    // Data
    TCPPacket* data_packet = get_base_tcp_packet(SIMPLE_TCP);
    data_packet->set_destination_port(5002);
    data_packet->set_source_port(1000);
    data_packet->set_tcp_ack(true);
    data_packet->set_tcp_sequence_number(3);
    data_packet->set_tcp_ack_number(2);
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
    expected.add_packet(ack); // </editor-fold>

    // Close
    // Active to Passive
    TCPPacket* fin1 = get_base_tcp_packet_ts(SIMPLE_TCP);
    fin1->set_tcp_sequence_number(4);
    fin1->set_tcp_ack_number(3);
    fin1->set_source_port(1000);
    fin1->set_destination_port(5002);
    fin1->set_tcp_fin(true);
    fin1->set_tcp_ack(true);

    // send
    expected.add_packet(fin1);
    // receive
    expected.add_packet(fin1);

    TCPPacket* ack1 = get_base_tcp_packet_ts(SIMPLE_TCP);
    ack1->set_tcp_sequence_number(3);
    ack1->set_tcp_ack_number(5);
    ack1->set_source_port(5002);
    ack1->set_destination_port(1000);
    ack1->set_tcp_ack(true);

    // send (drop)
    expected.add_packet(ack1);

    // Close
    // Passive to Active
    TCPPacket* fin2 = get_base_tcp_packet_ts(SIMPLE_TCP);
    fin2->set_tcp_sequence_number(4);
    fin2->set_tcp_ack_number(5);
    fin2->set_source_port(5002);
    fin2->set_destination_port(1000);
    fin2->set_tcp_fin(true);
    fin2->set_tcp_ack(true);

    // send
    expected.add_packet(fin2);

    // resend
    expected.add_packet(fin1);
    // receive (ignore)
    expected.add_packet(fin1);

    // resend
    expected.add_packet(fin2);
    // receive
    expected.add_packet(fin2);


    TCPPacket* ack2 = get_base_tcp_packet_ts(SIMPLE_TCP);
    ack2->set_tcp_sequence_number(5);
    ack2->set_tcp_ack_number(5);
    ack2->set_source_port(1000);
    ack2->set_destination_port(5002);
    ack2->set_tcp_ack(true);

    // send
    expected.add_packet(ack2);
    // receive
    expected.add_packet(ack2);

    compare_traces(expected);
}

TEST_F(BackEndMockTestDrop35Drop45, closeTestActiveToPassiveDrop35Drop45) {
    close_active_to_passive_drop_first_ack_and_second_fin();
}