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

#include "NetworkCallback.h"
#include "Semaphore.h"
#include "defines.h"
#include "PacketFactory.h"

using namespace std;


void* listener(void* arg);
void handle_receive(int fd, NetworkCallback* callback);

struct listen_data {
    NetworkCallback* callback;
    fd_set active_fd_set;
    fd_set read_fd_set;
    int max_fd;
    int min_fd;
    Semaphore* sem;
    map<int, PacketFactory*>* factories;
};

class RawSocketListener {
public:

    RawSocketListener() : max_fd_(-1), min_fd_(pow(2, 30)), started_(false) {
        FD_ZERO(&active_fd_set_);
        sem_ = new Semaphore();
        sem_->init(0);
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

        if (fd > max_fd_) {
            max_fd_ = fd;
        }
        if (fd < min_fd_) {
            min_fd_ = fd;
        }
        FD_SET(fd, &active_fd_set_);

        factories_[fd] = pf;
    }

    void start(NetworkCallback* callback) {
        started_ = true;
        max_fd_ += 1;

        struct listen_data data;
        data.active_fd_set = active_fd_set_;
        data.callback = callback;
        data.max_fd = max_fd_;
        data.min_fd = min_fd_;
        data.read_fd_set = read_fd_set_;
        data.sem = sem_;
        data.factories = &factories_;

        if (pthread_create(&thread_, NULL, &listener, &data) != 0) {
            perror("RawSocketListener: cannot create thread");
            exit(EXIT_FAILURE);
        }

        sem_->wait();
    }

private:
    map<int, int> fds_;
    map<int, PacketFactory*> factories_;
    fd_set active_fd_set_;
    fd_set read_fd_set_;
    int max_fd_;
    int min_fd_;
    Semaphore* sem_;
    bool started_;
    pthread_t thread_;
};

void* listener(void* arg) {
    struct listen_data* data = (struct listen_data*) arg;
    fd_set active_fd_set = data->active_fd_set;
    fd_set read_fd_set = data->read_fd_set;
    NetworkCallback* callback = data->callback;
    int max_fd = data->max_fd;
    int min_fd = data->min_fd;
    Semaphore* sem = data->sem;
    map<int, PacketFactory*>* factories = data->factories;

    sem->post();

    int ret;
    WiFuPacket* packet;
    PacketFactory* factory;
    while (1) {


        read_fd_set = active_fd_set;
        if (select(max_fd, &read_fd_set, NULL, NULL, NULL) < 0) {
            if(errno == EINTR) {
                continue;
            }
            perror("RawSocketListener: select()");
            exit(EXIT_FAILURE);
        }

        for (int fd = min_fd; fd < max_fd; ++fd) {
            if (FD_ISSET(fd, &read_fd_set)) {
                factory = factories->operator [](fd);
                packet = factory->create();
                ret = recv(fd, packet->get_payload(), PAYLOAD_SIZE, 0);
                if(ret <= 0) {
                    assert(false);
                }
                packet->set_ip_datagram_length(ret);
                callback->receive(packet);
            }
        }
    }
}

#endif	/* _RAWSOCKETLISTENER_H */

