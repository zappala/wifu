/* 
 * File:   RawSocketSender.h
 * Author: rbuck
 *
 * Created on January 28, 2011, 2:14 PM
 */

#ifndef _RAWSOCKETSENDER_H
#define	_RAWSOCKETSENDER_H

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

using namespace std;

class RawSocketSender {
public:

    RawSocketSender() {
        create_socket();
        set_header_include();
    }

    ssize_t send(WiFuPacket* p) {
        int ret = sendto(socket_,
                p->get_payload(),
                p->get_ip_length_bytes(),
                0,
                (struct sockaddr*) p->get_dest_address_port(),
                (sizeof (struct sockaddr_in)));

        if (ret < 0) {
            perror("NetworkInterface: Error Sending Packet");
            // TODO: What should we do on a fail?
        }
        return ret;
    }


private:
    int socket_;

    void create_socket() {
        socket_ = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
        if (socket_ < 0) {
            perror("NetworkInterface cannot create raw socket");
            exit(EXIT_FAILURE);
        }
    }

    void set_header_include() {
        int one = 1;
        const int *val = &one;
        if (setsockopt(socket_, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0) {
            perror("NetworkInterface: cannot set HDRINCL");
            exit(EXIT_FAILURE);
        }

    }



};

#endif	/* _RAWSOCKETSENDER_H */

