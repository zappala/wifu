/* 
 * File:   LocalSocketReceiver.h
 * Author: rbuck
 *
 * Created on October 19, 2010, 2:43 PM
 */

#ifndef _LOCALSOCKETRECEIVER_H
#define	_LOCALSOCKETRECEIVER_H

#include <iostream>
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

#include "LocalSocketReceiverCallback.h"
#include "Semaphore.h"
#include "defines.h"
#include "GarbageCollector.h"
#include "Utils.h"

#define MESSAGE_SIZE 2000

using namespace std;

/**
 * Handler used by a spawned thread to read from.
 */
void * unix_receive_handler(void * arg);

class LocalSocketReceiver;

/**
 * Data holder to pass information to the spawned thread running in unix_receive_handler().
 */
struct local_socket_receiver_obj {
    /**
     * Pointer to the LocalSocketReceiver which will handle incoming messages.
     */
    LocalSocketReceiver * receiver;

    /**
     * File descriptor of the socket to read from.
     */
    int sock;

    /**
     * Semaphore used as a signal indicating that the thread has started and
     * sucessfully copied all necessary data from this structure.
     */
    Semaphore sem;
};

/**
 * Class which wraps a unix socket and listens for messages.
 * A callback function is called upon receipt of any message.
 */
class LocalSocketReceiver {
public:
    LocalSocketReceiver(gcstring & file, LocalSocketReceiverCallback * callback);
    LocalSocketReceiver(const char* file, LocalSocketReceiverCallback * callback);

    virtual ~LocalSocketReceiver();
    int get_socket();

    gcstring & get_file();

    void recv(gcstring & message);
    virtual void receive(gcstring & message) = 0;



private:
    gcstring file_;
    Semaphore sem_;
    pthread_t thread_;
    LocalSocketReceiverCallback * callback_;
    int socket_;

    void init(void);
};


#endif	/* _LOCALSOCKETRECEIVER_H */

