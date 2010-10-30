/* 
 * File:   SendSynEvent.h
 * Author: rbuck
 *
 * Created on October 30, 2010, 9:58 AM
 */

#ifndef _SENDSYNEVENT_H
#define	_SENDSYNEVENT_H

#include "AddressPort.h"
#include "Event.h"

using namespace std;

class SendSynEvent : public Event, public AddressPort {

public:
    SendSynEvent(int socket, string & address, string & port) : Event(socket), AddressPort(address, port) {

    }

    void execute(IModule* m) {
        cout << "SendSynEvent Execute" << endl;
        m->udp_send(this);
    }
};

#endif	/* _SENDSYNEVENT_H */

