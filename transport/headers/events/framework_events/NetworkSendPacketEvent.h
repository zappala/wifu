/* 
 * File:   NetworkSendPacketEvent.h
 * Author: rbuck
 *
 * Created on January 28, 2011, 11:56 AM
 */

#ifndef _NETWORKSENDPACKETEVENT_H
#define	_NETWORKSENDPACKETEVENT_H

#include "PacketHolder.h"
#include "Socket.h"
#include "IModule.h"
#include "FrameworkEvent.h"

using namespace std;

/**
 * Event used to send a WiFuPacket to the INetworkInterface in use.
 *
 * @see INetworkInterface
 * @see NetworkInterface
 * @see FrameworkEvent
 * @see PacketHolder
 */
class NetworkSendPacketEvent : public PacketHolder, public FrameworkEvent {
public:

    /**
     * Constructs a NetworkSendPacketEvent.
     * @param socket The Socket object to which this Event belongs.
     * @param packet The packet to send.
     *
     * @see PacketHolder
     * @see FrameworkEvent
     */
    NetworkSendPacketEvent(Socket* socket, WiFuPacket* packet);

    /**
     * Destructor.
     */
    virtual ~NetworkSendPacketEvent();

    /**
     * Calls IModule::imodule_network_send() and passes this NetworkSendPacketEvent in as the argument.
     *
     * @param m The module which to call IModule::imodule_network_send() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_network_send()
     */
    void execute(IModule* m);

};

#endif	/* _NETWORKSENDPACKETEVENT_H */
