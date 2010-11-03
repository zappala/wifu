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

class UDPInterface : public Module {
public:
    UDPInterface() : Module() {

    }

    virtual ~UDPInterface() {

    }

    void udp_send(Event* e) {
        SendPacketEvent* event = (SendPacketEvent*)e;

        cout << "UDPInterface udp_send: " << event->get_address() << " " << event->get_port() << endl;
        
        if(e->get_socket() % 2 == 0) {
            // emulate a response
            usleep(100);
            dispatch(new PacketReceivedEvent(e->get_socket()));
            return;
        }
    }    
};

#endif	/* _UDPINTERFACE_H */

