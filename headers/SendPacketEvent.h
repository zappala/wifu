/* 
 * File:   SendPacketEvent.h
 * Author: rbuck
 *
 * Created on October 30, 2010, 9:58 AM
 */

#ifndef _SENDPACKETEVENT_H
#define	_SENDPACKETEVENT_H

#include "AddressPort.h"
#include "Event.h"

using namespace std;

class SendPacketEvent : public Event, public AddressPort {

public:
    SendPacketEvent(int socket, string & address, string & port) : Event(socket), AddressPort(address, port) {

    }

    void execute(IModule* m) {
        m->udp_send(this);
    }
};

#endif	/* _SENDSYNEVENT_H */

