/* 
 * File:   LocalSocketReceiver.h
 * Author: rbuck
 *
 * Created on October 19, 2010, 2:43 PM
 */

#ifndef _LOCALSOCKETRECEIVER_H
#define	_LOCALSOCKETRECEIVER_H

#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <pthread.h>
#include <semaphore.h>
#include <map>
#include <string.h>

#include "LocalSocketReceiverCallback.h"

#define MESSAGE_SIZE 2000

using namespace std;

void * thread_handler(void * arg);
void * thread_spawner(void * arg);

class LocalSocketReceiver;

struct thread_obj {
    LocalSocketReceiver * receiver;
};

struct thread_spawn_obj : thread_obj {
    int sock;
    sem_t sem;
};

struct thread_connection_handler_obj : thread_obj {
    int conn;
};

class LocalSocketReceiver {
public:
    LocalSocketReceiver(string & file, LocalSocketReceiverCallback * callback);
    virtual ~LocalSocketReceiver();

    string & getFile();
    int & getSocket();

    void recv(string &  message);
    virtual void receive(string &  message) = 0;

private:
    string file_;
    int socket_;
    sem_t sem_;
    pthread_t spawner_;
    LocalSocketReceiverCallback * callback_;

    void init(void);
};


#endif	/* _LOCALSOCKETRECEIVER_H */

