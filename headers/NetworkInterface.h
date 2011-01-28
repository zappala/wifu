/* 
 * File:   NetworkInterface.h
 * Author: rbuck
 *
 * Created on January 28, 2011, 11:50 AM
 */

#ifndef _NETWORKINTERFACE_H
#define	_NETWORKINTERFACE_H

#include "NetworkCallback.h"
#include "Module.h"

class NetworkInterface : public Module, public NetworkCallback {
public:

    static NetworkInterface& instance() {
        static NetworkInterface instance_;
        return instance_;
    }

    void start() {

    }

    virtual ~NetworkInterface() {

    }

    void receive(WiFuPacket* p) {

    }

    void network_send(Event* e) {
        
    }


};

#endif	/* _NETWORKINTERFACE_H */

