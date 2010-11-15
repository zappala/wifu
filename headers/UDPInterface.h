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

/**
 * Interface to an actual socket which data will be sent and recieved.
 */
class UDPInterface : public Module {
public:

    /**
     * Constructs a UDPInterface object.
     */
    UDPInterface() : Module() {

    }

    /**
     * Cleans up this UDPInterface object.
     */
    virtual ~UDPInterface() {

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
};

#endif	/* _UDPINTERFACE_H */

