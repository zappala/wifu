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
#include "events/PacketReceivedEvent.h"
#include "events/UDPSendPacketEvent.h"
#include "Packet.h"
#include "UDPSocket.h"
#include "UDPSocketCallback.h"
#include "AddressPort.h"
#include "events/UDPReceivePacketEvent.h"

/**
 * Interface to an actual socket which data will be sent and recieved.
 */
class UDPInterface : public Module, public UDPSocketCallback {
public:

    static UDPInterface& instance() {
        static UDPInterface instance_;
        return instance_;
    }

    /**
     * Starts the UDPInterface
     * Binds a UDP socket to the address/port specified in ap
     *
     * @param ap Contains the address and port which this UDPInterface will listen on
     */
    void start(AddressPort& ap) {
        // Set up the UDPSocket
        socket_.bind_socket(ap);
        socket_.receive(this);
    }


    /**
     * Cleans up this UDPInterface object.
     */
    virtual ~UDPInterface() {

    }

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
    void receive(AddressPort& ap, unsigned char* buffer, size_t length) {
        Packet* p = new Packet(buffer, length, ap.get_address(), socket_.get_bound_address_port()->get_address());
        Event* e = new UDPReceivePacketEvent(0, p);
        Dispatcher::instance().enqueue(e);
    }

    /**
     * This method is called when a UDPSendPacketEvent is dequeued from the Dispatcher.
     *
     * @param e The Event which contains information needing to be sent over the wire.
     */
    void udp_send(Event* e) {
        UDPSendPacketEvent* event = (UDPSendPacketEvent*) e;
        Packet* p = event->get_packet();
        AddressPort destination(p->get_dest_address(), p->get_dest_port());
        socket_.send(destination, p->to_bytes(), p->length());
    }

private:
    UDPSocket socket_;

    /**
     * Constructs a UDPInterface object.
     */
    UDPInterface() : Module(), UDPSocketCallback() {


    }
};

#endif	/* _UDPINTERFACE_H */

