/* 
 * File:   SocketConnectionManager.h
 * Author: rbuck
 *
 * Created on October 20, 2010, 11:18 AM
 */

#ifndef _SOCKETCONNECTIONMANAGER_H
#define	_SOCKETCONNECTIONMANAGER_H

#include "LocalSocketFullDuplex.h"
#include "ISocketModule.h"
#include "SocketUDPInterface.h"

/*
 * @depricated This class uses Unix sockets; we are now using Events instead.
 */
class SocketConnectionManager : public LocalSocketFullDuplex, public ISocketModule {
public:
    SocketConnectionManager(string & file) : LocalSocketFullDuplex(file){

    }

    ~SocketConnectionManager() {
        
    }

    void receive(string & message) {
        cout << "Connection manager Receive: " << message << endl;
    }

    void connect(string & message) {
        cout << "Connection manager connect: " << message << endl;
        send_to(SocketUDPInterface::instance()->getFile(), message);
    }

    void data(string & message) {
        cout << "Connection manager data: " << message << endl;
    }
};


#endif	/* _SOCKETCONNECTIONMANAGER_H */

