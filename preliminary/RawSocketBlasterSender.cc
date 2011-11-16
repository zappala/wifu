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
#include "Timer.h"
#include "OptionParser.h"

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
    return ret;
}

/*
 * 
 */
int main(int argc, char** argv) {
//    cout << "Sender" << endl;

    // parameters:
    // destination address
    // rate (goodput), long
    // duration (seconds), long

    gcstring des = "dest";
    gcstring r = "rate";
    gcstring dur = "duration";

    static struct option long_options[] = {
        {des.c_str(), required_argument, NULL, 0},
        {r.c_str(), required_argument, NULL, 0},
        {dur.c_str(), required_argument, NULL, 0},
        {0, 0, 0, 0}
    };

    int protocol = 250;
    int dest_port = 50000;
    //    gcstring dest_address = "5.0.0.54";
    gcstring dest_address = "127.0.0.1";
    long duration = 10.0; // seconds
    long rate = 200.0; // Mbps (goodput)

    OptionParser::instance().parse(argc, argv, long_options);
    if (OptionParser::instance().present(des)) {
        dest_address = OptionParser::instance().argument(des);
    }
    if (OptionParser::instance().present(r)) {
        rate = atof(OptionParser::instance().argument(r).c_str());
    }
    if (OptionParser::instance().present(dur)) {
        duration = atof(OptionParser::instance().argument(dur).c_str());
    }

    AddressPort* address = new AddressPort(dest_address, dest_port);
    struct sockaddr* dest = (struct sockaddr*) address->get_network_struct_ptr();

    TCPPacket* temp = new TCPPacket();
    temp->insert_tcp_header_option(new TCPTimestampOption);

    TCPPacket* p = new TCPPacket();
    p->set_ip_protocol(protocol);
    p->set_destination_port(dest_port);
    p->set_ip_destination_address_s(dest_address);   
    p->insert_tcp_header_option(new TCPTimestampOption);

    gcstring to_send = RandomStringGenerator::get_data(p->max_data_length());
    p->set_data((unsigned char*) to_send.data(), to_send.size());


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



    u_int64_t total_bits = ((rate * 1000000) * duration);
    u_int64_t packet_bits = p->get_data_length_bytes() * 8;
    u_int64_t total_packets = total_bits / packet_bits;
    double chunks = 100.0;
    double packets_per_chunk = total_packets / chunks;
    double wait = (duration / chunks) * 1000000;

    cout << total_bits << endl;
    cout << packet_bits << endl;
    cout << total_packets << endl;
    cout << packets_per_chunk << endl;
    cout << wait << endl;

    int total_bytes_sent = 0;

    Timer timer;
    timer.start();
    for (int i = 0; i < chunks; ++i) {
        for (int j = 0; j < packets_per_chunk; ++j) {
            raw_send(s, p, dest);
            total_bytes_sent += p->get_data_length_bytes();
        }
        usleep(wait/2);
    }
    timer.stop();

    cout << "sent " << total_bytes_sent << endl;
    cout << "duration " << timer.get_duration_seconds() << endl;
    double send_rate = ((total_bytes_sent * 8) / timer.get_duration_seconds()) / 1000000;
    cout << "rate " << send_rate << endl;


    close(s);
    return 0;
}

