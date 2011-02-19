/* 
 * File:   UDPInterface.h
 * Author: rbuck
 *
 * Created on October 30, 2010, 9:37 AM
 */

#ifndef _UDPINTERFACE_H
#define	_UDPINTERFACE_H

#include "Module.h"
#include "Dispatcher.h"
#include "events/TimeoutEvent.h"
#include "events/UDPSendPacketEvent.h"
#include "UDPSocket.h"
#include "UDPSocketCallback.h"
#include "AddressPort.h"
#include "events/UDPReceivePacketEvent.h"
#include "SocketCollection.h"

/**
 * Interface to an actual socket which data will be sent and received.
 */
class UDPInterface : public Module, public UDPSocketCallback {
public:
    static UDPInterface& instance();

    /**
     * Starts the UDPInterface
     * Binds a UDP socket to the address/port specified in ap
     *
     * @param ap Contains the address and port which this UDPInterface will listen on
     */
    void start(AddressPort& ap);

    /**
     * Cleans up this UDPInterface object.
     */
    virtual ~UDPInterface();

    /**
     * Implementation of callback funtion defined in UDPSocketCallback
     * This method is called whenever data is received from the UDPSocket
     *
     * @param ap Object containing the address and port of the sending socket
     * @param buffer The data which is being received
     * @param length The length of buffer
     *
     * @see UDPSocketCallback::receive()
     */
    void receive(AddressPort& ap, unsigned char* buffer, size_t length);

    /**
     * This method is called when a UDPSendPacketEvent is dequeued from the Dispatcher.
     *
     * @param e The Event which contains information needing to be sent over the wire.
     */
    void udp_send(Event* e);

    string& get_bound_ip_address();

private:
    UDPSocket socket_;

    /**
     * Constructs a UDPInterface object.
     */
    UDPInterface();
};

#endif	/* _UDPINTERFACE_H */

