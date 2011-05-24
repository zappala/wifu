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

void* tcp_tahoe_close_send_receive_thread(void* args) {

    struct var* v = (struct var*) args;
    AddressPort* to_bind = v->to_bind_;
    Semaphore* sem = v->sem_;

    string message = v->expected_string;

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
    string address("127.0.0.1");
    string res = ap.get_address();
    EXPECT_EQ(address, res);
    //    cout << "Connected to: " << ap.to_s() << endl;

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
    wifu_close(connection);
}

/**
 * @param num_bytes The number of bytes to send, currently, this is also the number of packets to send (we sent one data byte per packet)
 *
 */
void tcp_tahoe_close_send_receive_test(string message) {
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


    if (pthread_create(&(t), NULL, &tcp_tahoe_close_send_receive_thread, &(v)) != 0) {
        FAIL() << "Error creating new thread in IntegrationTest.h";
    }

    v.sem_->wait();

    // Make sure that the thread is in the accept state
    usleep(50000);

    // Create client

    timer.start();
    client = wifu_socket(AF_INET, SOCK_STREAM, TCP_TAHOE);
    result = wifu_connect(client, (const struct sockaddr *) to_connect.get_network_struct_ptr(), sizeof (struct sockaddr_in));
    timer.stop();
    EXPECT_EQ(0, result);

    //    cout << "Duration (us) to create a socket and connect on localhost via wifu: " << timer.get_duration_microseconds() << endl;

    int size = 1500;
    char buffer[size];
    memset(buffer, 0, size);
    string expected = v.expected_string;
    string all_received = "";

    while (true) {

        int return_value = wifu_recv(client, &buffer, 1, 0);

        if (return_value == 0) {
            break;
        }

        string actual(buffer);
        all_received.append(actual);

    }

    EXPECT_EQ(expected, all_received);
    wifu_close(client);
    cout << "IntegrationTest::send_receive_test(), received the following: " << all_received << endl;
    sleep(10);
}

void tcp_tahoe_close_passive_to_active_drop_none() {
    // <editor-fold defaultstate="collapsed" desc="setup">
    string data = random_string(1);
    tcp_tahoe_close_send_receive_test(data);

    NetworkTrace expected;
    // Connect

    // Send
    TCPPacket* syn = get_syn(TCP_TAHOE);
    syn->set_tcp_receive_window_size(MAX_TCP_RECEIVE_WINDOW_SIZE);
    expected.add_packet(syn);
    // receive
    expected.add_packet(syn);

    // send
    TCPPacket* synack = get_synack(TCP_TAHOE);
    synack->set_tcp_receive_window_size(MAX_TCP_RECEIVE_WINDOW_SIZE);
    expected.add_packet(synack);
    // receive
    expected.add_packet(synack);

    TCPPacket* ack = get_base_tcp_packet_ts(TCP_TAHOE);
    ack->set_tcp_receive_window_size(MAX_TCP_RECEIVE_WINDOW_SIZE);
    ack->set_tcp_sequence_number(2);
    ack->set_tcp_ack_number(2);
    ack->set_source_port(1000);
    ack->set_destination_port(5002);
    ack->set_tcp_ack(true);

    // send
    expected.add_packet(ack);
    // receive
    expected.add_packet(ack);

    // Data

    TCPPacket* data_packet = get_base_tcp_packet(TCP_TAHOE);
    data_packet->set_tcp_receive_window_size(MAX_TCP_RECEIVE_WINDOW_SIZE);
    data_packet->set_destination_port(1000);
    data_packet->set_source_port(5002);
    data_packet->set_tcp_ack(true);
    data_packet->set_tcp_sequence_number(2);
    data_packet->set_tcp_ack_number(2);
    data_packet->insert_tcp_header_option(new TCPTimestampOption());
    data_packet->set_data((unsigned char*) data.c_str(), data.size());

    // send
    expected.add_packet(data_packet);
    // receive
    expected.add_packet(data_packet);

    // </editor-fold>

    // Close
    // Passive to Active
    TCPPacket* fin1 = get_base_tcp_packet_ts(TCP_TAHOE);
    fin1->set_tcp_receive_window_size(MAX_TCP_RECEIVE_WINDOW_SIZE);
    fin1->set_tcp_sequence_number(3);
    fin1->set_tcp_ack_number(2);
    fin1->set_source_port(5002);
    fin1->set_destination_port(1000);
    fin1->set_tcp_fin(true);
    fin1->set_tcp_ack(true);



    TCPPacket* ack_for_data = get_base_tcp_packet_ts(TCP_TAHOE);
    ack_for_data->set_tcp_receive_window_size(MAX_TCP_RECEIVE_WINDOW_SIZE);
    ack_for_data->set_tcp_sequence_number(2);
    ack_for_data->set_tcp_ack_number(3);
    ack_for_data->set_source_port(1000);
    ack_for_data->set_destination_port(5002);
    ack_for_data->set_tcp_ack(true);


    // send
    expected.add_packet(ack_for_data);
    // receive
    expected.add_packet(ack_for_data);

    // send
    expected.add_packet(fin1);
    // receive
    expected.add_packet(fin1);


    TCPPacket* ack_for_fin = get_base_tcp_packet_ts(TCP_TAHOE);
    ack_for_fin->set_tcp_receive_window_size(MAX_TCP_RECEIVE_WINDOW_SIZE);
    ack_for_fin->set_tcp_sequence_number(2);
    ack_for_fin->set_tcp_ack_number(4);
    ack_for_fin->set_source_port(1000);
    ack_for_fin->set_destination_port(5002);
    ack_for_fin->set_tcp_ack(true);

    // send
    expected.add_packet(ack_for_fin);
    // receive
    expected.add_packet(ack_for_fin);

    // Close
    // Active to Passive
    TCPPacket* fin2 = get_base_tcp_packet_ts(TCP_TAHOE);
    fin2->set_tcp_receive_window_size(MAX_TCP_RECEIVE_WINDOW_SIZE);
    fin2->set_tcp_sequence_number(2);
    fin2->set_tcp_ack_number(4);
    fin2->set_source_port(1000);
    fin2->set_destination_port(5002);
    fin2->set_tcp_fin(true);
    fin2->set_tcp_ack(true);

    // send
    expected.add_packet(fin2);
    // receive
    expected.add_packet(fin2);


    TCPPacket* ack2 = get_base_tcp_packet_ts(TCP_TAHOE);
    ack2->set_tcp_receive_window_size(MAX_TCP_RECEIVE_WINDOW_SIZE);
    ack2->set_tcp_sequence_number(4);
    ack2->set_tcp_ack_number(3);
    ack2->set_source_port(5002);
    ack2->set_destination_port(1000);
    ack2->set_tcp_ack(true);

    // send
    expected.add_packet(ack2);
    // receive
    expected.add_packet(ack2);

    compare_traces(expected);
}

TEST_F(BackEndMockTestDropNone, tahoeCloseTestPassiveToActive) {
    tcp_tahoe_close_passive_to_active_drop_none();
}

void tcp_tahoe_close_passive_to_active_drop_first_fin() {
     // <editor-fold defaultstate="collapsed" desc="setup">
    string data = random_string(1);
    tcp_tahoe_close_send_receive_test(data);

    NetworkTrace expected;
    // Connect

    // Send
    TCPPacket* syn = get_syn(TCP_TAHOE);
    syn->set_tcp_receive_window_size(MAX_TCP_RECEIVE_WINDOW_SIZE);
    expected.add_packet(syn);
    // receive
    expected.add_packet(syn);

    // send
    TCPPacket* synack = get_synack(TCP_TAHOE);
    synack->set_tcp_receive_window_size(MAX_TCP_RECEIVE_WINDOW_SIZE);
    expected.add_packet(synack);
    // receive
    expected.add_packet(synack);

    TCPPacket* ack = get_base_tcp_packet_ts(TCP_TAHOE);
    ack->set_tcp_receive_window_size(MAX_TCP_RECEIVE_WINDOW_SIZE);
    ack->set_tcp_sequence_number(2);
    ack->set_tcp_ack_number(2);
    ack->set_source_port(1000);
    ack->set_destination_port(5002);
    ack->set_tcp_ack(true);

    // send
    expected.add_packet(ack);
    // receive
    expected.add_packet(ack);

    // Data

    TCPPacket* data_packet = get_base_tcp_packet(TCP_TAHOE);
    data_packet->set_tcp_receive_window_size(MAX_TCP_RECEIVE_WINDOW_SIZE);
    data_packet->set_destination_port(1000);
    data_packet->set_source_port(5002);
    data_packet->set_tcp_ack(true);
    data_packet->set_tcp_sequence_number(2);
    data_packet->set_tcp_ack_number(2);
    data_packet->insert_tcp_header_option(new TCPTimestampOption());
    data_packet->set_data((unsigned char*) data.c_str(), data.size());

    // send
    expected.add_packet(data_packet);
    // receive
    expected.add_packet(data_packet);

    // </editor-fold>

    // Close
    // Passive to Active
    TCPPacket* fin1 = get_base_tcp_packet_ts(TCP_TAHOE);
    fin1->set_tcp_receive_window_size(MAX_TCP_RECEIVE_WINDOW_SIZE);
    fin1->set_tcp_sequence_number(3);
    fin1->set_tcp_ack_number(2);
    fin1->set_source_port(5002);
    fin1->set_destination_port(1000);
    fin1->set_tcp_fin(true);
    fin1->set_tcp_ack(true);



    TCPPacket* ack_for_data = get_base_tcp_packet_ts(TCP_TAHOE);
    ack_for_data->set_tcp_receive_window_size(MAX_TCP_RECEIVE_WINDOW_SIZE);
    ack_for_data->set_tcp_sequence_number(2);
    ack_for_data->set_tcp_ack_number(3);
    ack_for_data->set_source_port(1000);
    ack_for_data->set_destination_port(5002);
    ack_for_data->set_tcp_ack(true);


    // send
    expected.add_packet(ack_for_data);
    // receive
    expected.add_packet(ack_for_data);

    // send
    expected.add_packet(fin1);
    // resend
    expected.add_packet(fin1);
    // receive
    expected.add_packet(fin1);

    TCPPacket* ack_for_fin = get_base_tcp_packet_ts(TCP_TAHOE);
    ack_for_fin->set_tcp_receive_window_size(MAX_TCP_RECEIVE_WINDOW_SIZE);
    ack_for_fin->set_tcp_sequence_number(2);
    ack_for_fin->set_tcp_ack_number(4);
    ack_for_fin->set_source_port(1000);
    ack_for_fin->set_destination_port(5002);
    ack_for_fin->set_tcp_ack(true);

    // send
    expected.add_packet(ack_for_fin);
    // receive
    expected.add_packet(ack_for_fin);

    // Close
    // Active to Passive
    TCPPacket* fin2 = get_base_tcp_packet_ts(TCP_TAHOE);
    fin2->set_tcp_receive_window_size(MAX_TCP_RECEIVE_WINDOW_SIZE);
    fin2->set_tcp_sequence_number(2);
    fin2->set_tcp_ack_number(4);
    fin2->set_source_port(1000);
    fin2->set_destination_port(5002);
    fin2->set_tcp_fin(true);
    fin2->set_tcp_ack(true);

    // send
    expected.add_packet(fin2);
    // receive
    expected.add_packet(fin2);


    TCPPacket* ack2 = get_base_tcp_packet_ts(TCP_TAHOE);
    ack2->set_tcp_receive_window_size(MAX_TCP_RECEIVE_WINDOW_SIZE);
    ack2->set_tcp_sequence_number(4);
    ack2->set_tcp_ack_number(3);
    ack2->set_source_port(5002);
    ack2->set_destination_port(1000);
    ack2->set_tcp_ack(true);

    // send
    expected.add_packet(ack2);
    // receive
    expected.add_packet(ack2);

    compare_traces(expected);
}

TEST_F(BackEndMockTestDrop32, tahoeCloseTestPassiveToActiveDropFirstFin) {
    tcp_tahoe_close_passive_to_active_drop_first_fin();
}

//void tcp_tahoe_close_passive_to_active_drop_first_ack() {
//    // <editor-fold defaultstate="collapsed" desc="setup">
//    string data = random_string(1);
//    tcp_tahoe_close_send_receive_test(data);
//
//    NetworkTrace expected;
//
//    // Connect
//
//    // Send
//    expected.add_packet(get_syn(TCP_TAHOE));
//    // receive
//    expected.add_packet(get_syn(TCP_TAHOE));
//
//    // send
//    expected.add_packet(get_synack(TCP_TAHOE));
//    // receive
//    expected.add_packet(get_synack(TCP_TAHOE));
//
//    // send
//    expected.add_packet(get_ack(TCP_TAHOE));
//    // receive
//    expected.add_packet(get_ack(TCP_TAHOE));
//
//    // Data
//
//    TCPPacket* data_packet = get_base_tcp_packet(TCP_TAHOE);
//    data_packet->set_destination_port(1000);
//    data_packet->set_source_port(5002);
//    data_packet->set_tcp_ack(true);
//    data_packet->set_tcp_sequence_number(2);
//    data_packet->set_tcp_ack_number(3);
//    data_packet->insert_tcp_header_option(new TCPTimestampOption());
//    data_packet->set_data((unsigned char*) data.c_str(), data.size());
//
//    // send
//    expected.add_packet(data_packet);
//    // receive
//    expected.add_packet(data_packet);
//
//    TCPPacket* ack = get_base_tcp_packet_ts(TCP_TAHOE);
//    ack->set_tcp_sequence_number(3);
//    ack->set_tcp_ack_number(3);
//    ack->set_source_port(1000);
//    ack->set_destination_port(5002);
//    ack->set_tcp_ack(true);
//
//    // send
//    expected.add_packet(ack);
//    // receive
//    expected.add_packet(ack); // </editor-fold>
//
//    // Close
//    // Passive to Active
//    TCPPacket* fin1 = get_base_tcp_packet_ts(TCP_TAHOE);
//    fin1->set_tcp_sequence_number(3);
//    fin1->set_tcp_ack_number(4);
//    fin1->set_source_port(5002);
//    fin1->set_destination_port(1000);
//    fin1->set_tcp_fin(true);
//    fin1->set_tcp_ack(true);
//
//    // send
//    expected.add_packet(fin1);
//    // receive
//    expected.add_packet(fin1);
//
//    TCPPacket* ack1 = get_base_tcp_packet_ts(TCP_TAHOE);
//    ack1->set_tcp_sequence_number(4);
//    ack1->set_tcp_ack_number(4);
//    ack1->set_source_port(1000);
//    ack1->set_destination_port(5002);
//    ack1->set_tcp_ack(true);
//
//    // send (drop)
//    expected.add_packet(ack1);
//
//    // Close
//    // Active to Passive
//    TCPPacket* fin2 = get_base_tcp_packet_ts(TCP_TAHOE);
//    fin2->set_tcp_sequence_number(5);
//    fin2->set_tcp_ack_number(4);
//    fin2->set_source_port(1000);
//    fin2->set_destination_port(5002);
//    fin2->set_tcp_fin(true);
//    fin2->set_tcp_ack(true);
//
//    // send
//    expected.add_packet(fin2);
//    // receive
//    expected.add_packet(fin2);
//
//
//    TCPPacket* ack2 = get_base_tcp_packet_ts(TCP_TAHOE);
//    ack2->set_tcp_sequence_number(4);
//    ack2->set_tcp_ack_number(6);
//    ack2->set_source_port(5002);
//    ack2->set_destination_port(1000);
//    ack2->set_tcp_ack(true);
//
//    // send
//    expected.add_packet(ack2);
//    // receive
//    expected.add_packet(ack2);
//
//    compare_traces(expected);
//}
//
//TEST_F(BackEndMockTestDrop44, tahoeCloseTestPassiveToActiveDrop44) {
//    tcp_tahoe_close_passive_to_active_drop_first_ack();
//}
//
//void tcp_tahoe_close_passive_to_active_drop_second_fin() {
//    // <editor-fold defaultstate="collapsed" desc="setup">
//    string data = random_string(1);
//    tcp_tahoe_close_send_receive_test(data);
//
//    NetworkTrace expected;
//
//    // Connect
//
//    // Send
//    expected.add_packet(get_syn(TCP_TAHOE));
//    // receive
//    expected.add_packet(get_syn(TCP_TAHOE));
//
//    // send
//    expected.add_packet(get_synack(TCP_TAHOE));
//    // receive
//    expected.add_packet(get_synack(TCP_TAHOE));
//
//    // send
//    expected.add_packet(get_ack(TCP_TAHOE));
//    // receive
//    expected.add_packet(get_ack(TCP_TAHOE));
//
//    // Data
//
//    TCPPacket* data_packet = get_base_tcp_packet(TCP_TAHOE);
//    data_packet->set_destination_port(1000);
//    data_packet->set_source_port(5002);
//    data_packet->set_tcp_ack(true);
//    data_packet->set_tcp_sequence_number(2);
//    data_packet->set_tcp_ack_number(3);
//    data_packet->insert_tcp_header_option(new TCPTimestampOption());
//    data_packet->set_data((unsigned char*) data.c_str(), data.size());
//
//    // send
//    expected.add_packet(data_packet);
//    // receive
//    expected.add_packet(data_packet);
//
//    TCPPacket* ack = get_base_tcp_packet_ts(TCP_TAHOE);
//    ack->set_tcp_sequence_number(3);
//    ack->set_tcp_ack_number(3);
//    ack->set_source_port(1000);
//    ack->set_destination_port(5002);
//    ack->set_tcp_ack(true);
//
//    // send
//    expected.add_packet(ack);
//    // receive
//    expected.add_packet(ack); // </editor-fold>
//
//    // Close
//    // Passive to Active
//    TCPPacket* fin1 = get_base_tcp_packet_ts(TCP_TAHOE);
//    fin1->set_tcp_sequence_number(3);
//    fin1->set_tcp_ack_number(4);
//    fin1->set_source_port(5002);
//    fin1->set_destination_port(1000);
//    fin1->set_tcp_fin(true);
//    fin1->set_tcp_ack(true);
//
//    // send
//    expected.add_packet(fin1);
//    // receive
//    expected.add_packet(fin1);
//
//    TCPPacket* ack1 = get_base_tcp_packet_ts(TCP_TAHOE);
//    ack1->set_tcp_sequence_number(4);
//    ack1->set_tcp_ack_number(4);
//    ack1->set_source_port(1000);
//    ack1->set_destination_port(5002);
//    ack1->set_tcp_ack(true);
//
//    // send
//    expected.add_packet(ack1);
//    // receive
//    expected.add_packet(ack1);
//
//    // Close
//    // Active to Passive
//    TCPPacket* fin2 = get_base_tcp_packet_ts(TCP_TAHOE);
//    fin2->set_tcp_sequence_number(5);
//    fin2->set_tcp_ack_number(4);
//    fin2->set_source_port(1000);
//    fin2->set_destination_port(5002);
//    fin2->set_tcp_fin(true);
//    fin2->set_tcp_ack(true);
//
//    // send
//    expected.add_packet(fin2);
//    // resend
//    expected.add_packet(fin2);
//    // receive
//    expected.add_packet(fin2);
//
//
//    TCPPacket* ack2 = get_base_tcp_packet_ts(TCP_TAHOE);
//    ack2->set_tcp_sequence_number(4);
//    ack2->set_tcp_ack_number(6);
//    ack2->set_source_port(5002);
//    ack2->set_destination_port(1000);
//    ack2->set_tcp_ack(true);
//
//    // send
//    expected.add_packet(ack2);
//    // receive
//    expected.add_packet(ack2);
//
//    compare_traces(expected);
//}
//
//TEST_F(BackEndMockTestDrop54, tahoeCloseTestPassiveToActiveDrop54) {
//    tcp_tahoe_close_passive_to_active_drop_second_fin();
//}
//
//void tcp_tahoe_close_passive_to_active_drop_second_ack() {
//    // <editor-fold defaultstate="collapsed" desc="setup">
//    string data = random_string(1);
//    tcp_tahoe_close_send_receive_test(data);
//
//    NetworkTrace expected;
//
//    // Connect
//
//    // Send
//    expected.add_packet(get_syn(TCP_TAHOE));
//    // receive
//    expected.add_packet(get_syn(TCP_TAHOE));
//
//    // send
//    expected.add_packet(get_synack(TCP_TAHOE));
//    // receive
//    expected.add_packet(get_synack(TCP_TAHOE));
//
//    // send
//    expected.add_packet(get_ack(TCP_TAHOE));
//    // receive
//    expected.add_packet(get_ack(TCP_TAHOE));
//
//    // Data
//
//    TCPPacket* data_packet = get_base_tcp_packet(TCP_TAHOE);
//    data_packet->set_destination_port(1000);
//    data_packet->set_source_port(5002);
//    data_packet->set_tcp_ack(true);
//    data_packet->set_tcp_sequence_number(2);
//    data_packet->set_tcp_ack_number(3);
//    data_packet->insert_tcp_header_option(new TCPTimestampOption());
//    data_packet->set_data((unsigned char*) data.c_str(), data.size());
//
//    // send
//    expected.add_packet(data_packet);
//    // receive
//    expected.add_packet(data_packet);
//
//    TCPPacket* ack = get_base_tcp_packet_ts(TCP_TAHOE);
//    ack->set_tcp_sequence_number(3);
//    ack->set_tcp_ack_number(3);
//    ack->set_source_port(1000);
//    ack->set_destination_port(5002);
//    ack->set_tcp_ack(true);
//
//    // send
//    expected.add_packet(ack);
//    // receive
//    expected.add_packet(ack); // </editor-fold>
//
//    // Close
//    // Passive to Active
//    TCPPacket* fin1 = get_base_tcp_packet_ts(TCP_TAHOE);
//    fin1->set_tcp_sequence_number(3);
//    fin1->set_tcp_ack_number(4);
//    fin1->set_source_port(5002);
//    fin1->set_destination_port(1000);
//    fin1->set_tcp_fin(true);
//    fin1->set_tcp_ack(true);
//
//    // send
//    expected.add_packet(fin1);
//    // receive
//    expected.add_packet(fin1);
//
//    TCPPacket* ack1 = get_base_tcp_packet_ts(TCP_TAHOE);
//    ack1->set_tcp_sequence_number(4);
//    ack1->set_tcp_ack_number(4);
//    ack1->set_source_port(1000);
//    ack1->set_destination_port(5002);
//    ack1->set_tcp_ack(true);
//
//    // send
//    expected.add_packet(ack1);
//    // receive
//    expected.add_packet(ack1);
//
//    // Close
//    // Active to Passive
//    TCPPacket* fin2 = get_base_tcp_packet_ts(TCP_TAHOE);
//    fin2->set_tcp_sequence_number(5);
//    fin2->set_tcp_ack_number(4);
//    fin2->set_source_port(1000);
//    fin2->set_destination_port(5002);
//    fin2->set_tcp_fin(true);
//    fin2->set_tcp_ack(true);
//
//    // send
//    expected.add_packet(fin2);
//    // receive
//    expected.add_packet(fin2);
//
//
//    TCPPacket* ack2 = get_base_tcp_packet_ts(TCP_TAHOE);
//    ack2->set_tcp_sequence_number(4);
//    ack2->set_tcp_ack_number(6);
//    ack2->set_source_port(5002);
//    ack2->set_destination_port(1000);
//    ack2->set_tcp_ack(true);
//
//    // send
//    expected.add_packet(ack2);
//
//    // resend
//    expected.add_packet(fin2);
//    // receive
//    expected.add_packet(fin2);
//
//    // resend
//    expected.add_packet(ack2);
//    // receive
//    expected.add_packet(ack2);
//
//    compare_traces(expected);
//}
//
//TEST_F(BackEndMockTestDrop46, tahoeCloseTestPassiveToActiveDrop46) {
//    tcp_tahoe_close_passive_to_active_drop_second_ack();
//}
//
//void tcp_tahoe_close_passive_to_active_drop_first_ack_and_second_fin() {
//    // <editor-fold defaultstate="collapsed" desc="setup">
//    string data = random_string(1);
//    tcp_tahoe_close_send_receive_test(data);
//
//    NetworkTrace expected;
//
//    // Connect
//
//    // Send
//    expected.add_packet(get_syn(TCP_TAHOE));
//    // receive
//    expected.add_packet(get_syn(TCP_TAHOE));
//
//    // send
//    expected.add_packet(get_synack(TCP_TAHOE));
//    // receive
//    expected.add_packet(get_synack(TCP_TAHOE));
//
//    // send
//    expected.add_packet(get_ack(TCP_TAHOE));
//    // receive
//    expected.add_packet(get_ack(TCP_TAHOE));
//
//    // Data
//
//    TCPPacket* data_packet = get_base_tcp_packet(TCP_TAHOE);
//    data_packet->set_destination_port(1000);
//    data_packet->set_source_port(5002);
//    data_packet->set_tcp_ack(true);
//    data_packet->set_tcp_sequence_number(2);
//    data_packet->set_tcp_ack_number(3);
//    data_packet->insert_tcp_header_option(new TCPTimestampOption());
//    data_packet->set_data((unsigned char*) data.c_str(), data.size());
//    // send
//    expected.add_packet(data_packet);
//    // receive
//    expected.add_packet(data_packet);
//
//    TCPPacket* ack = get_base_tcp_packet_ts(TCP_TAHOE);
//    ack->set_tcp_sequence_number(3);
//    ack->set_tcp_ack_number(3);
//    ack->set_source_port(1000);
//    ack->set_destination_port(5002);
//    ack->set_tcp_ack(true);
//
//    // send
//    expected.add_packet(ack);
//    // receive
//    expected.add_packet(ack); // </editor-fold>
//
//    // Close
//    // Passive to Active
//    TCPPacket* fin1 = get_base_tcp_packet_ts(TCP_TAHOE);
//    fin1->set_tcp_sequence_number(3);
//    fin1->set_tcp_ack_number(4);
//    fin1->set_source_port(5002);
//    fin1->set_destination_port(1000);
//    fin1->set_tcp_fin(true);
//    fin1->set_tcp_ack(true);
//
//    // send
//    expected.add_packet(fin1);
//    // receive
//    expected.add_packet(fin1);
//
//    TCPPacket* ack1 = get_base_tcp_packet_ts(TCP_TAHOE);
//    ack1->set_tcp_sequence_number(4);
//    ack1->set_tcp_ack_number(4);
//    ack1->set_source_port(1000);
//    ack1->set_destination_port(5002);
//    ack1->set_tcp_ack(true);
//
//    // send (drop)
//    expected.add_packet(ack1);
//
//    // Close
//    // Active to Passive
//    TCPPacket* fin2 = get_base_tcp_packet_ts(TCP_TAHOE);
//    fin2->set_tcp_sequence_number(5);
//    fin2->set_tcp_ack_number(4);
//    fin2->set_source_port(1000);
//    fin2->set_destination_port(5002);
//    fin2->set_tcp_fin(true);
//    fin2->set_tcp_ack(true);
//
//    // send
//    expected.add_packet(fin2);
//
//    // resend
//    expected.add_packet(fin1);
//    // receive (ignore)
//    expected.add_packet(fin1);
//
//    // resend
//    expected.add_packet(fin2);
//    // receive
//    expected.add_packet(fin2);
//
//    TCPPacket* ack2 = get_base_tcp_packet_ts(TCP_TAHOE);
//    ack2->set_tcp_sequence_number(4);
//    ack2->set_tcp_ack_number(6);
//    ack2->set_source_port(5002);
//    ack2->set_destination_port(1000);
//    ack2->set_tcp_ack(true);
//
//    // send
//    expected.add_packet(ack2);
//    // receive
//    expected.add_packet(ack2);
//
//    compare_traces(expected);
//}
//
//TEST_F(BackEndMockTestDrop44Drop54, tahoeCloseTestPassiveToActiveDrop44Drop54) {
//    tcp_tahoe_close_passive_to_active_drop_first_ack_and_second_fin();
//}
