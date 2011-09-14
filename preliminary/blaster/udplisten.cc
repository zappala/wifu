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
// UDP listener
//
// This program will listen to incoming UDP packets from a remote
// machine.  The program listens on a message socket to receive
// commands:
//
//    start : Start listening for packets until a second passes without
//            any incoming packets.  Send back on the message socket the
//            number of packets recived.

//    quit  : Exit the program
//

// listen for incoming UDP packets
//
// udp = socket to listen on
int listener(int udp) {
    int count = 0;
    while (1) {
        /* get a packet */
        int buflen = 1000;
        char buf[buflen];
        memset(buf,0,buflen);
        ssize_t num = recvfrom(udp,&buf,buflen,0,NULL,NULL);
        if (num < 0) {
            if (errno == EINTR)
                continue;
            if (errno == EAGAIN)
                break;
            perror("recvfrom");
            exit(1);
        }
        count += 1;
    }
    return count;
}

int
main(int argc, char **argv) 
{
    struct sockaddr_in sin;
    int udp;
    int msg;
    string host = "any";
    int port = 5000;
    string message_host = "any";
    int message_port = 6000;
    int option_index;
    struct hostent *hostEntry;

    // options
    // host = host name to send packets to
    // port = port to send packets to
    // message_host = host name to bind on to receive control messages
    // message_port = port to bind on to receive control messages
    static struct option long_options[] = {
        {"host",required_argument, NULL, 0},
        {"port",required_argument, NULL, 0},
        {"message_host",required_argument, NULL, 0},
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
                if (!strcmp(long_options[option_index].name,"host")) {
                    host = string(optarg);
                }
                if (!strcmp(long_options[option_index].name,"port")) {
                    port = atoi(optarg);
                }
                if (!strcmp(long_options[option_index].name,"message-host")) {
                    message_host = string(optarg);
                }
                if (!strcmp(long_options[option_index].name,"message-port")) {
                    message_port = atoi(optarg);
                }

                break;

            case '?':
                printf("udplisten\n");
                break;
     
            default:
                break;
        }
    }

    // use DNS to get host name
    if (host != "any") {
        hostEntry = gethostbyname(host.c_str());
        if (!hostEntry) {
            perror("No such host name");
            exit(-1);
        }
    }

    // setup address for UDP socket
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    if (host == "any")
        sin.sin_addr.s_addr = INADDR_ANY;
    else
        memcpy(&sin.sin_addr, hostEntry->h_addr_list[0], hostEntry->h_length);

    // create and bind UDP socket
    if ((udp = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("UDP socket");
        exit(1);
    }
    if (bind(udp,(const struct sockaddr *)&sin,sizeof(sin)) < 0) {
        perror("bind");
        exit(-1);
    }

    // set timeout for UDP socket
    struct timeval tv;
    tv.tv_usec = 0;
    tv.tv_sec = 1;
    if (setsockopt(udp, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,  sizeof tv))
    {
        perror("setsockopt");
        return -1;
    }

        // use DNS to get host name
    if (message_host != "any") {
        hostEntry = gethostbyname(message_host.c_str());
        if (!hostEntry) {
            perror("No such host name");
            exit(-1);
        }
    }
    
    // setup address for message socket
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(message_port);
    if (message_host == "any")
        sin.sin_addr.s_addr = INADDR_ANY;
    else
        memcpy(&sin.sin_addr, hostEntry->h_addr_list[0], hostEntry->h_length);

    // create and bind message socket
    if ((msg = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("UDP socket");
        exit(1);
    }
    if (bind(msg,(const struct sockaddr *)&sin,sizeof(sin)) < 0) {
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
        if (tokens.size() < 1)
            break;
        if (tokens.at(0) == "quit")
            break;

        // do the listening
        int result = listener(udp);
        memset(buf,0,buflen);
        sprintf(buf,"%d",result);
        if (sendto(msg,buf,strlen(buf),0,(struct sockaddr *)&from,fromlen) < 0) {
            perror("sendto");
            exit(1);
        }
    }
}
