/* 
 * File:   PacketReceivedEvent.h
 * Author: rbuck
 *
 * Created on November 3, 2010, 4:20 PM
 */

#ifndef _PACKETRECEIVEDEVENT_H
#define	_PACKETRECEIVEDEVENT_H

#include "Event.h"

class PacketReceivedEvent : public Event {
public:
    PacketReceivedEvent(int socket) : Event(socket) {

    }

    virtual ~PacketReceivedEvent() {

    }

    void execute(IModule* m) {
        m->receive(this);
    }

};

#endif	/* _PACKETRECEIVEDEVENT_H */

