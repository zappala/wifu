/* 
 * File:   ConnectEvent.h
 * Author: rbuck
 *
 * Created on October 29, 2010, 2:27 PM
 */

#ifndef _CONNECTEVENT_H
#define	_CONNECTEVENT_H

#include "Event.h"
#include "ISocketModule.h"
#include "AddressPort.h"


class ConnectEvent : public Event, public AddressPort {

public:
    ConnectEvent(int socket, string & address, string & port)
        : Event(socket), AddressPort(address, port) {

    }

    void execute(IModule* m) {
        m->connect(this);
    }
};

#endif	/* _CONNECTEVENT_H */

