/* 
 * File:   SendPacketEvent.h
 * Author: rbuck
 *
 * Created on October 30, 2010, 9:58 AM
 */

#ifndef _SENDPACKETEVENT_H
#define	_SENDPACKETEVENT_H

#include "Event.h"
#include "packet/WiFuPacket.h"

using namespace std;

/**
 * Event which represents the sending of a packet.
 * @see Event
 */
class SendPacketEvent : public Event {
public:

    /**
     * Constructs a SendPacketEvent.
     *
     * @param socket The socket, which represents a unique connection, to use for this Event
     * @param packet The WiFuPacket object to send
     */
    SendPacketEvent(Socket* socket, WiFuPacket* packet) : Event(socket), packet_(packet) {

    }

    /**
     * Will call send() on m.
     *
     * @param m The IModule which to call send() on.
     * @see IModule::send()
     */
    void execute(IModule* m) {
        m->send(this);
    }

    /**
     * @return The WiFuPacket to send
     */
    WiFuPacket* get_packet() {
        return packet_;
    }

private:
    /**
     * Pointer to the packet to be sent
     */
    WiFuPacket* packet_;
};

#endif	/* _SENDSYNEVENT_H */

