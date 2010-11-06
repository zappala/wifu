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

/**
 * Event which represents the sending of a packet.
 * @see Event
 */
class SendPacketEvent : public Event, public AddressPort {
public:

    /**
     * Constructs a SendPacketEvent.
     *
     * @param socket The socket, which represents a unique connection, to use for this Event
     * @param address The IP address of the machine to send to.
     * @param port The port to send to.
     */
    SendPacketEvent(int socket, string & address, string & port) : Event(socket), AddressPort(address, port) {

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
};

#endif	/* _SENDSYNEVENT_H */

