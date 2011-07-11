/* 
 * File:   RawSocketListener.h
 * Author: rbuck
 *
 * Created on January 28, 2011, 1:18 PM
 */

#ifndef _RAWSOCKETLISTENER_H
#define	_RAWSOCKETLISTENER_H

#include <map>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <string.h>
#include <iostream>
#include <errno.h>
#include <math.h>
#include <sys/epoll.h>

#include "NetworkCallback.h"
#include "Semaphore.h"
#include "defines.h"
#include "PacketFactory.h"

#define MAX_EVENTS 100

using namespace std;

void* listener(void* arg);

struct listen_data {
    NetworkCallback* callback;
    int epfd;
    Semaphore* sem;
    map<int, PacketFactory*, std::less<int>, gc_allocator<std::pair<int, PacketFactory*> > >* factories;
};

class RawSocketListener {
public:

    RawSocketListener();

    void register_protocol(int protocol, PacketFactory* pf);

    void start(NetworkCallback* callback);

private:
    map<int, PacketFactory*, std::less<int>, gc_allocator<std::pair<int, PacketFactory*> > > factories_;
    int epfd_;
    Semaphore* sem_;
    bool started_;
    pthread_t thread_;
};

#endif	/* _RAWSOCKETLISTENER_H */

