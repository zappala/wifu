/* 
 * File:   UDPSocket.h
 * Author: rbuck
 *
 * Created on June 15, 2010, 3:22 PM
 */

#ifndef _UDPSOCKET_H
#define	_UDPSOCKET_H

#include <stdlib.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include <vector>
#include <pthread.h>

#include "Semaphore.h"
#include "UDPSocketCallback.h"
#include "AddressPort.h"

using namespace std;

#define BUF_LENGTH 4096

struct socket_thread_data {
    int socket;
    UDPSocketCallback* callback;
    Semaphore* sem;
};


void* receive_handler(void* arg);

class UDPSocket {
public:
    UDPSocket();
    virtual ~UDPSocket();

    // General Functions
    
    int closeSocket();
    int getSocket() const;
    void makeNonBlocking();

    // Sending Functions
    size_t send(AddressPort& ap, string& message);
    size_t send(AddressPort& ap, const unsigned char* message, socklen_t length);
    size_t send(struct sockaddr_in* address, const unsigned char* message, socklen_t length);

    // Listening Functions
    void bind_socket(const string& address, int port);
    void set_receive_timeout(int sec);
    void receive(UDPSocketCallback* callback);

private:
    pthread_t receive_thread_;
    int sock_;
    Semaphore* sem_;

    int createSocket();
};

#endif	/* _UDPSOCKET_H */

