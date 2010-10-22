/* 
 * File:   ConnectionManager.h
 * Author: rbuck
 *
 * Created on October 20, 2010, 11:18 AM
 */

#ifndef _CONNECTIONMANAGER_H
#define	_CONNECTIONMANAGER_H

#include "LocalSocketFullDuplex.h"
#include "IModule.h"
#include "UDPInterface.h"

class ConnectionManager : public LocalSocketFullDuplex, public IModule {
public:
    ConnectionManager(string & file) : LocalSocketFullDuplex(file){

    }

    ~ConnectionManager() {
        
    }

    void receive(string & message) {
        cout << "Connection manager Receive: " << message << endl;
    }

    void connect(string & message) {
        cout << "Connection manager connect: " << message << endl;
        send_to(UDPInterface::instance()->getFile(), message);
    }

    void data(string & message) {
        cout << "Connection manager data: " << message << endl;
    }
};


#endif	/* _CONNECTIONMANAGER_H */

