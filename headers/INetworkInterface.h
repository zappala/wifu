/* 
 * File:   INetworkInterface.h
 * Author: rbuck
 *
 * Created on February 9, 2011, 3:54 PM
 */

#ifndef _INETWORKINTERFACE_H
#define	_INETWORKINTERFACE_H

#include "PacketFactory.h"
#include "Module.h"
#include "NetworkCallback.h"

#include "events/NetworkSendPacketEvent.h"
#include "SocketCollection.h"

/**
 * Interface which must be implemented by all classes wishing to send packets.
 * This is part of the Mock Object pattern for the NetworkInterface
 *
 * @see NetworkInterface
 * @see INetworkInterfaceCreator
 * @see Module
 * @see NetworkCallback
 */
class INetworkInterface : public Module, public NetworkCallback {
public:

    /**
     * Constructs a new INetworkInterface
     */
    INetworkInterface() : Module(), NetworkCallback() {}

    /**
     * Tells this INetworkInterface to start listening for packets.
     */
    virtual void start() = 0;

    /**
     * Registers protocols with a PacketFactory.
     * This is used so the correct packet can be used from the start (upon receiving data at the network level).
     * This ensures us we only copy the buffer once in the back-end.
     * If a packet is received with protocol in its ip header, pf is used to create the appropriate packet type.
     * For example, TCP is protocol 6 and we need TCPPacket types for this protocol.
     * So I would call this method as follows: register_protocol(6, new TCPPacketFactory())
     * This also allows us to create new protocols (and associated packet types).
     *
     * @param protocol The protocol to use as a key to know which PacketFactory to use when a packet is received.
     * @param pf The PacketFactory implementation to use which returns the correct type of packet for protocol.
     * 
     * @see PacketFactory
     * @see TCPPacketFactory
     */
    virtual void register_protocol(int protocol, PacketFactory* pf) = 0;

    /**
     * Extracts the WiFuPacket from e and sends it to whatever network is underlying this class.
     *
     * @param e The NetworkSendPacketEvent
     *
     * @see NetworkSendPacketEvent
     */
    virtual void network_send(Event* e) = 0;
};

#endif	/* _INETWORKINTERFACE_H */

