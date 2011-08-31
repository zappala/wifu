/* 
 * File:   RawSocketBlasterReceiver.cc
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
#include <signal.h>

#include "../headers/packet/TCPPacket.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    cout << "Receiver" << endl;

    int protocol = 250;

    int fd = socket(AF_INET, SOCK_RAW, protocol);
    if (fd < 0) {
        perror("RawSocketListener: Cannot create socket");
        exit(EXIT_FAILURE);
    }

    int optval = 1000000;
    int value = setsockopt(fd, IPPROTO_IP, SO_RCVBUF, &optval, sizeof (optval));
    if (value) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    

    int num, total_bytes;
    while (true) {
        TCPPacket* p = new TCPPacket();
        num = recv(fd, p->get_payload(), MTU, 0);
        if (num < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("recv()");
            exit(EXIT_FAILURE);
        }

        total_bytes += p->get_data_length_bytes();
        //cout << total_bytes << endl;

        if(p->is_tcp_fin()) {
            break;
        }
    }

    cout << total_bytes << endl;

    close(fd);

    return 0;
}

