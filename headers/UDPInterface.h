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
        WiFuPacket* p = new WiFuPacket(buffer, length);
        AddressPort* remote = p->get_source_address_port();
        AddressPort* local = p->get_dest_address_port();

        Socket* s = SocketCollection::instance().get_by_local_and_remote_ap(local, remote);

        if(!s) {
            s = SocketCollection::instance().get_by_local_ap(local);
        }


        if(!s) {
            // No bound local socket
            return;
        }

        
        
        Event* e = new UDPReceivePacketEvent(s, p);
        Dispatcher::instance().enqueue(e);
    }

    /**
     * This method is called when a UDPSendPacketEvent is dequeued from the Dispatcher.
     *
     * @param e The Event which contains information needing to be sent over the wire.
     */
    void udp_send(Event* e) {
        UDPSendPacketEvent* event = (UDPSendPacketEvent*) e;
        WiFuPacket* p = event->get_packet();
        string dest = p->get_ip_destination_address_s();
        AddressPort destination(dest, WIFU_PORT);
        socket_.send(destination, p->get_payload(), p->get_ip_datagram_length());
    }

    string& get_bound_ip_address() {
        return socket_.get_bound_address_port()->get_address();
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

