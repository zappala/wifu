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
#include <vector>

#include "LocalSocketReceiverCallback.h"
#include "Semaphore.h"
#include "defines.h"

#define MESSAGE_SIZE 2000

using namespace std;

void * unix_receive_handler(void * arg);

class LocalSocketReceiver;

struct local_socket_receiver_obj {
    LocalSocketReceiver * receiver;
    int sock;
    Semaphore sem;
};

class LocalSocketReceiver {
public:
    LocalSocketReceiver(string & file, LocalSocketReceiverCallback * callback);
    LocalSocketReceiver(const char* file, LocalSocketReceiverCallback * callback);

    virtual ~LocalSocketReceiver();
    int get_socket();

    string & getFile();

    void recv(string & message);
    virtual void receive(string & message) = 0;



private:
    string file_;
    Semaphore sem_;
    pthread_t thread_;
    LocalSocketReceiverCallback * callback_;
    int socket_;

    void init(void);
};


#endif	/* _LOCALSOCKETRECEIVER_H */

