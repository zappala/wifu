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

#include "packet/WiFuPacket.h"

using namespace std;

class RawSocketSender {
public:
    RawSocketSender();

    ssize_t send(WiFuPacket*);

private:
    int socket_;

    void create_socket();

    void set_header_include();
};

#endif	/* _RAWSOCKETSENDER_H */

