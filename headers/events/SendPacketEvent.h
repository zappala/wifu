/* 
 * File:   SendPacketEvent.h
 * Author: rbuck
 *
 * Created on October 30, 2010, 9:58 AM
 */

#ifndef _SENDPACKETEVENT_H
#define	_SENDPACKETEVENT_H

#include "Event.h"
#include "Packet.h"

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
     * @param packet The Packet object to send
     */
    SendPacketEvent(int socket, Packet* packet) : Event(socket), packet_(packet) {

    }

    /**
     * Will call udp_send() on m.
     *
     * @param m The IModule which to call udp_send() on.
     * @see IModule::udp_send()
     */
    void execute(IModule* m) {
        m->udp_send(this);
    }

    /**
     * @return The Packet to send
     */
    Packet* get_packet() {
        return packet_;
    }

private:
    /**
     * Pointer to the packet to be sent
     */
    Packet* packet_;
};

#endif	/* _SENDSYNEVENT_H */

