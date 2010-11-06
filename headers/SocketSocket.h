/* 
 * File:   SocketSocket.h
 * Author: rbuck
 *
 * Created on October 20, 2010, 11:46 AM
 */

#ifndef _SOCKETSOCKET_H
#define	_SOCKETSOCKET_H

#include "LocalSocketSender.h"
#include "SocketDispatcher.h"


#include <string>

using namespace std;

/*
 * @depricated This class uses Unix sockets; we are now using Events instead.
 */
class SocketSocket : public LocalSocketFullDuplex {
public:

    SocketSocket(string & file) : LocalSocketFullDuplex(file) {
        
    }

    void connect(string & address) {

    }

    void send(string & message) {
        
    }

    void receive(string &  message) {

    }

};

#endif	/* _SOCKETSOCKET_H */

