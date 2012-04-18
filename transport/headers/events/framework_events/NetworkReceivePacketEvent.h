/* 
 * File:   NetworkReceivePacketEvent.h
 * Author: rbuck
 *
 * Created on January 28, 2011, 11:59 AM
 */

#ifndef _NETWORKRECEIVEPACKETEVENT_H
#define	_NETWORKRECEIVEPACKETEVENT_H

#include "PacketHolder.h"
#include "Socket.h"
#include "FrameworkEvent.h"

using namespace std;

/**
 * Event created and dispatched when an INetworkInterface receives a packet.
 *
 * @see INetworkInterface
 * @see NetworkInterface
 * @see FrameworkEvent
 * @see PacketHolder
 */
class NetworkReceivePacketEvent : public PacketHolder, public FrameworkEvent {
public:

    /**
     * Constructs a NetworkReceivePacketEvent.
     * @param socket The Socket object to which this Event belongs.
     * @param packet The packet received.
     * It is likely that it is actually a child class of WiFuPacket.
     *
     * @see PacketHolder
     * @see FrameworkEvent
     */
    NetworkReceivePacketEvent(Socket* socket, WiFuPacket* packet);

    /**
     * Destructor.
     */
    virtual ~NetworkReceivePacketEvent();

    /**
     * Calls IModule::imodule_network_receive() and passes this NetworkReceivePacketEvent in as the argument.
     *
     * @param m The module which to call IModule::imodule_network_receive() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_network_receive()
     */
    void execute(IModule* m);

};

#endif	/* _NETWORKRECEIVEPACKETEVENT_H */
