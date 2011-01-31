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

#define MAX_EVENTS 1000

using namespace std;


void* listener(void* arg);
void handle_receive(int fd, NetworkCallback* callback);

struct listen_data {
    NetworkCallback* callback;
    int epfd;
    Semaphore* sem;
    map<int, PacketFactory*>* factories;
};

class RawSocketListener {
public:

    RawSocketListener() : started_(false) {
        sem_ = new Semaphore();
        sem_->init(0);
        epfd_ = epoll_create(1);
    }

    void register_protocol(int protocol, PacketFactory* pf) {
        if (started_) {
            cout << "Cannot register anymore protocols" << endl;
            // TODO: throw an exception
            return;
        }

        int fd = socket(AF_INET, SOCK_RAW, protocol);
        if (fd < 0) {
            perror("RawSocketListener: Cannot create socket");
            exit(EXIT_FAILURE);
        }

        struct epoll_event event;
        event.events = EPOLLIN;
        event.data.fd = fd;
        epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &event);

        factories_[fd] = pf;
    }

    void start(NetworkCallback* callback) {
        started_ = true;

        struct listen_data data;
        data.callback = callback;
        data.epfd = epfd_;
        data.sem = sem_;
        data.factories = &factories_;

        if (pthread_create(&thread_, NULL, &listener, &data) != 0) {
            perror("RawSocketListener: cannot create thread");
            exit(EXIT_FAILURE);
        }

        sem_->wait();
    }

private:
    map<int, PacketFactory*> factories_;
    int epfd_;
    Semaphore* sem_;
    bool started_;
    pthread_t thread_;
};

void* listener(void* arg) {
    struct listen_data* data = (struct listen_data*) arg;
    NetworkCallback* callback = data->callback;
    int epfd = data->epfd;
    Semaphore* sem = data->sem;
    map<int, PacketFactory*>* factories = data->factories;

    sem->post();

    int ret;
    WiFuPacket* packet;
    PacketFactory* factory;
    struct epoll_event events[MAX_EVENTS];
    int nfds, fd;

    while (1) {
        nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);

        if (nfds < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("RawSocketListener: epoll_wait()");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < nfds; ++i) {

            fd = events[i].data.fd;

            factory = factories->operator [](fd);
            packet = factory->create();
            
            ret = recv(fd, packet->get_payload(), PAYLOAD_SIZE, 0);
            if (ret <= 0) {
                assert(false);
            }
            packet->set_ip_datagram_length(ret);
            callback->receive(packet);
        }
    }
}

#endif	/* _RAWSOCKETLISTENER_H */

