/* 
 * File:   LocalSocketSender.h
 * Author: rbuck
 *
 * Created on October 19, 2010, 2:43 PM
 */

#ifndef _LOCALSOCKETSENDER_H
#define	_LOCALSOCKETSENDER_H

#include <string>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <map>

#include "Semaphore.h"

using namespace std;

class LocalSocketSender {
public:
    LocalSocketSender();
    virtual ~LocalSocketSender();

    void send_to(string & socket_file, string & message);
private:
    map<string, int> sockets_;
    
    void send_to(int & socket, string & message);
    void create_socket(string & socket_file);

    Semaphore mutex_;
    Semaphore mutex1_;
};

#endif	/* _LOCALSOCKETSENDER_H */

