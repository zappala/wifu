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
#include "Timer.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
//    cout << "Receiver" << endl;

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

    // set timeout for socket
    struct timeval tv;
    tv.tv_usec = 0;
    tv.tv_sec = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,  sizeof tv))
    {
        perror("setsockopt");
        return -1;
    }

    

    Timer timer;
    int num, total_bytes = 0;
    TCPPacket* p = new TCPPacket();
    while (true) {        
        num = recv(fd, p->get_payload(), MTU, 0);
        timer.start();
        timer.update_stop();
        if (num < 0) {
            if (errno == EINTR) {
                continue;
            }
            else if(errno == EAGAIN) {
                break;
            }
            perror("recv()");
            exit(EXIT_FAILURE);
        }
        total_bytes += p->get_data_length_bytes();
    }
    

    cout << "received " << total_bytes << endl;
    double time_ = timer.get_duration_seconds() - tv.tv_sec;
    cout << "duration " << time_ << endl;
    double receive_rate = ((total_bytes * 8) / time_) / 1000000;
    cout << "rate " << receive_rate << endl;

    close(fd);

    return 0;
}

