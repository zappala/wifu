/* 
 * File:   SendPacketEvent.h
 * Author: rbuck
 *
 * Created on October 30, 2010, 9:58 AM
 */

#ifndef _SENDPACKETEVENT_H
#define	_SENDPACKETEVENT_H

#include "events/PacketEvent.h"
#include "packet/WiFuPacket.h"

using namespace std;

/**
 * Event which represents the sending of a packet.
 * @see PacketEvent
 */
class SendPacketEvent : public PacketEvent {
public:
    /**
     * Constructs a SendPacketEvent.
     *
     * @param socket The socket, which represents a unique connection, to use for this Event
     * @param packet The WiFuPacket object to send
     */
    SendPacketEvent(Socket*, WiFuPacket*);

    /**
     * Will call send() on m.
     *
     * @param m The IModule which to call send() on.
     * @see IModule::send()
     */
    void execute(IModule*);

};

#endif	/* _SENDSYNEVENT_H */
