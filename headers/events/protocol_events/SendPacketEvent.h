/* 
 * File:   SendPacketEvent.h
 * Author: rbuck
 *
 * Created on October 30, 2010, 9:58 AM
 */

#ifndef _SENDPACKETEVENT_H
#define	_SENDPACKETEVENT_H

#include "PacketHolder.h"
#include "packet/WiFuPacket.h"
#include "ProtocolEvent.h"

using namespace std;

/**
 * Event which represents the sending of a packet.
 * @see PacketEvent
 */
class SendPacketEvent : public PacketHolder, public ProtocolEvent {
public:
    /**
     * Constructs a SendPacketEvent.
     *
     * @param socket The socket, which represents a unique connection, to use for this Event
     * @param packet The WiFuPacket object to send
     */
    SendPacketEvent(Socket* socket, WiFuPacket* packet);

    /**
     * Will call send() on m.
     *
     * @param m The IModule which to call send() on.
     * @see IModule::send()
     */
    void execute(IModule* m);

};

#endif	/* _SENDSYNEVENT_H */
