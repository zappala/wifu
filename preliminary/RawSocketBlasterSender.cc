/* 
 * File:   RawSocketBlasterSender.cc
 * Author: rbuck
 *
 * Created on August 16, 2011, 11:20 AM
 */

#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>

#include "../headers/packet/TCPPacket.h"
#include "packet/TCPTimestampOption.h"
#include "../test/headers/RandomStringGenerator.h"

using namespace std;

int raw_send(int socket, TCPPacket* p, struct sockaddr* dest) {
    int ret = sendto(socket,
            p->get_payload(),
            p->get_ip_tot_length(),
            0,
            dest,
            (sizeof (struct sockaddr_in)));

    if (ret < 0) {
        perror("RawSocketSender: Error Sending Packet");
        // TODO: What should we do on a fail?
    }
}

/*
 * 
 */
int main(int argc, char** argv) {
    cout << "Sender" << endl;

    int protocol = 250;
    int dest_port = 50000;
    //gcstring dest_address = "192.168.21.103";
    gcstring dest_address = "127.0.0.1";

    AddressPort* address = new AddressPort(dest_address, dest_port);
    struct sockaddr* dest = (struct sockaddr*) address->get_network_struct_ptr();

    int num_bytes_to_send = 1000000;
    gcstring to_send = RandomStringGenerator::get_data(num_bytes_to_send);
    TCPPacket* temp = new TCPPacket();
    temp->insert_tcp_header_option(new TCPTimestampOption);
    float max_packet_size = temp->max_data_length();

    // create and set the data in all the packets
    int num_packets = ceil((float) num_bytes_to_send / max_packet_size);

    list<TCPPacket*, gc_allocator<TCPPacket*> > packets;
    for (int i = 0; i < num_packets; ++i) {

        TCPPacket* p = new TCPPacket();
        p->set_ip_protocol(protocol);
        p->set_destination_port(dest_port);
        p->set_ip_destination_address_s(dest_address);
        p->insert_tcp_header_option(new TCPTimestampOption);

        gcstring data = to_send.substr(0, (int) max_packet_size);
        to_send.erase(0, max_packet_size);

        p->set_data((unsigned char*) data.data(), data.size());

        packets.push_back(p);
    }

    TCPPacket* fin = *(packets.rbegin());
    fin->set_tcp_fin(true);

    int s = socket(AF_INET, SOCK_RAW, protocol);
    if (s < 0) {
        perror("Cannot create raw socket");
        exit(EXIT_FAILURE);
    }

    int one = 1;
    const int *val = &one;
    if (setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0) {
        perror("Cannot set HDRINCL");
        exit(EXIT_FAILURE);
    }

    socklen_t optval = num_bytes_to_send;
    int value = setsockopt(s, IPPROTO_IP, SO_SNDBUF, &optval, sizeof (optval));
    if (value) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    list<TCPPacket*, gc_allocator<TCPPacket*> >::iterator itr = packets.begin();

    for (; itr != packets.end(); ++itr) {
        raw_send(s, *itr, dest);
        //usleep(130);

    }

    close(s);
    return 0;
}

