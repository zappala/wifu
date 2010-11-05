/* 
 * File:   PacketReceivedEvent.h
 * Author: rbuck
 *
 * Created on November 3, 2010, 4:20 PM
 */

#ifndef _PACKETRECEIVEDEVENT_H
#define	_PACKETRECEIVEDEVENT_H

#include "Event.h"

/**
 * Event which represents receipt of a packet.
 *
 * @see Event
 */
class PacketReceivedEvent : public Event {
public:

    /**
     * Constructs a PacketReceivedEvent.
     *
     * @param socket The socket which received a packet.
     */
    PacketReceivedEvent(int socket) : Event(socket) {

    }

    /**
     * Cleans up a PacketReceivedEvent.
     */
    virtual ~PacketReceivedEvent() {

    }

    /**
     * Will call receive(this) on m.
     *
     * @param m The module which to call receive() on.
     * @see IModule::receive()
     */
    void execute(IModule* m) {
        m->receive(this);
    }

};

#endif	/* _PACKETRECEIVEDEVENT_H */

