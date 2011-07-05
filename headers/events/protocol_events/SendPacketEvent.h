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
 * This is meant to be processed by a Protocol, not by a INetworkInterface.
 * A Protocol will probably dispatch a NetworkSendPacketEvent after processing the internal packet.
 * That is, this will not trigger a packet to be sent over the network interface.
 * For a packet to be sent to the network interface use NetworkSendPacketEvent.
 *
 * @see Event
 * @see ProtocolEvent
 * @see PacketHolder
 * @see NetworkSendPacketEvent
 */
class SendPacketEvent : public PacketHolder, public ProtocolEvent {
public:
    /**
     * Constructs a SendPacketEvent.
     *
     * @param socket The Socket object to which this Event belongs.
     * @param packet The WiFuPacket to process.
     */
    SendPacketEvent(Socket* socket, WiFuPacket* packet);

    /**
     * Calls IModule::imodule_send() and passes this SendPacketEvent in as the argument.
     *
     * @param m The module which to call IModule::imodule_send() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_send()
     */
    void execute(IModule* m);

};

#endif	/* _SENDSYNEVENT_H */
