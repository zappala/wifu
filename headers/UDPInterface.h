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
#include "TimeoutEvent.h"
#include "PacketReceivedEvent.h"
#include "SendPacketEvent.h"
#include "Packet.h"
#include "UDPSocket.h"
#include "UDPSocketCallback.h"
#include "AddressPort.h"

/**
 * Interface to an actual socket which data will be sent and recieved.
 */
class UDPInterface : public Module , public UDPSocketCallback {
public:

    /**
     * Constructs a UDPInterface object.
     */
    UDPInterface(string& address, int port) : Module(), UDPSocketCallback() {

        // Set up the UDPSocket
        socket_.bind_socket(address, port);
        socket_.makeNonBlocking();
        socket_.receive(this);

    }

    /**
     * Cleans up this UDPInterface object.
     */
    virtual ~UDPInterface() {
        socket_.closeSocket();
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
        //Packet p = new Packet()
    }

    /**
     * This method is called when a SendPacketEvent is dequeued from the Dispatcher.
     *
     * @param e The Event which contains information needing to be sent over the wire.
     */
    void udp_send(Event* e) {
        SendPacketEvent* event = (SendPacketEvent*)e;

        cout << "UDPInterface udp_send: " << event->get_packet()->to_bytes() << endl;
        
        if(e->get_socket() % 2 == 0) {
            // emulate a response
            usleep(100);
            dispatch(new PacketReceivedEvent(e->get_socket()));
            return;
        }
    }

private:
    UDPSocket socket_;
};

#endif	/* _UDPINTERFACE_H */

