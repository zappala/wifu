#include <arpa/inet.h>
#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>
#include "Tokenizer.h"

using namespace std;

//
// UDP blaster
//
// This program will send UDP packets to a listening application on a remote
// machine.  The program listens on a message socket to receive commands:
//
//    start <IP> <port> <size> <rate> <duration>
//          : Start sending packets to <IP> listening on <port>.  The packets
//            should be <size> bytes and should be sent at <rate> packets per
//            second for <duration> seconds.  Send back on the message socket
//            the number of packets sent.
//
//    quit  : Exit the program
//


// blast UDP packets at another machine
//
// sin = address to send packets to
// size = size of the packet in bytes
// rate = rate in packets per second
// duration = number of seconds
long blast(int udp, struct sockaddr_in *sin, int size, int rate, int duration) {

    // allocate packet
    size -= 50;
    if (size <= 0)
        return 0;
    
    u_char* packet = (u_char *)malloc(size);
    int i;
    for (i=0; i<=size; i++)
        packet[i] = 's';

    // divide packets into 100 bunches
    int scale = 100;
    int bunches = rate/scale;
    double wait = 1000000*(1.0/scale);

    // send bunches of packets for the requested duration
    int count = 0;
    while (count < scale*duration) {
        // send a bunch
        for (i=0; i<bunches; i++) {
            if (sendto(udp, packet, size, 0,
                       (struct sockaddr *)sin, sizeof(struct sockaddr)) < 0)  {
                perror("sendto");
                return 0;
            }
        }
        usleep(wait);
        count += 1;
    }

    // return the number of packets sent
    long packets = (rate/scale)*count;
    return packets;
}

int
main(int argc, char **argv) 
{
    struct sockaddr_in sin_udp;
    struct sockaddr_in sin_msg;
    int udp;
    int msg;
    string message_host = "any";
    int message_port = 6000;
    string ip;
    int port;
    int size = 64;
    int rate = 10;
    int duration = 1;
    int option_index;
    struct hostent *hostEntry;

    // options
    // message-host = host name to bind on to receive control messages
    // message-port = port to bind on to receive control messages
    static struct option long_options[] = {
        {"message-host",required_argument, NULL, 0},
        {"message-port",required_argument, NULL, 0},
        {0, 0, 0, 0},
    };

    // parse options
    while (1) {
        option_index = 0;

        int c = getopt_long (argc, argv, "", long_options, &option_index);
     
        // detect the end of the options
        if (c == -1) {
            break;
        }

        switch (c) {
            case 0:
                if (!strcmp(long_options[option_index].name,"message-host")) {
                    message_host = string(optarg);
                    break;
                }
                if (!strcmp(long_options[option_index].name,"message-port")) {
                    message_port = atoi(optarg);
                }
                break;

            case '?':
                printf("udpblast\n");
                break;
     
            default:
                break;
        }
    }

    // create udp socket
    if ((udp = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("UDP socket");
        exit(1);
    }

    // use DNS to get host name for binding to message socket
    if (message_host != "any") {
        hostEntry = gethostbyname(message_host.c_str());
        if (!hostEntry) {
            perror("No such host name");
            exit(-1);
        }
    }
    
    // setup address for message socket
    memset(&sin_msg, 0, sizeof(sin_msg));
    sin_msg.sin_family = AF_INET;
    sin_msg.sin_port = htons(message_port);
    if (message_host == "any")
        sin_msg.sin_addr.s_addr = INADDR_ANY;
    else
        memcpy(&sin_msg.sin_addr, hostEntry->h_addr_list[0], hostEntry->h_length);

    // create and bind message socket
    if ((msg = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("UDP socket");
        exit(1);
    }
    if (bind(msg,(const struct sockaddr *)&sin_msg,sizeof(sin_msg)) < 0) {
        perror("bind");
        exit(-1);
    }

    // wait for commands
    while (1) {
        // get a command
        int buflen = 1000;
        char buf[buflen];
        memset(buf,0,buflen);
        struct sockaddr_in from;
        socklen_t fromlen = sizeof(struct sockaddr_in);
        ssize_t num = recvfrom(msg,&buf,buflen,0,
                               (struct sockaddr *)&from,&fromlen);
        if (num < 0) {
            if (errno == EINTR)
                continue;
            if (errno == EAGAIN)
                break;
            perror("recvfrom");
            exit(1);
        }
        // tokenize the string
        Tokenizer tokenizer;
        string p = string(buf);
        vector<string> tokens = tokenizer.parse(p, " ");
        if (tokens.size() < 6)
            // must be a quit message
            break;

        // get start message parameters
        ip = tokens.at(1);
        port = atoi(tokens.at(2).c_str());
        size = atoi(tokens.at(3).c_str());
        rate = atoi(tokens.at(4).c_str());
        duration = atoi(tokens.at(5).c_str());

        // setup address for UDP socket
        memset(&sin_udp, 0, sizeof(sin_udp));
        sin_udp.sin_family = AF_INET;
        sin_udp.sin_port = htons(port);
        if (!inet_aton(ip.c_str(),&sin_udp.sin_addr))
            continue;

        // do the blast
        long result = blast(udp,&sin_udp,size,rate,duration);
        memset(buf,0,buflen);
        sprintf(buf,"%li",result);
        if (sendto(msg,buf,strlen(buf),0,(struct sockaddr *)&from,fromlen) < 0) {
            perror("sendto");
            exit(1);
        }
    }
}

