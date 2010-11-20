/* 
 * File:   UDPSendPacketEvent.h
 * Author: rbuck
 *
 * Created on November 19, 2010, 2:52 PM
 */

#ifndef _UDPSENDPACKETEVENT_H
#define	_UDPSENDPACKETEVENT_H

#include "PacketEvent.h"

class UDPSendPacketEvent : public PacketEvent {
public:
    UDPSendPacketEvent(int socket, Packet* packet) : PacketEvent(socket, packet) {

    }

    virtual ~UDPSendPacketEvent() {

    }
    
    void execute(IModule* m) {
        m->udp_send(this);
    }


};

#endif	/* _UDPSENDPACKETEVENT_H */

