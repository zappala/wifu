/* 
 * File:   UDPReceivePacketEvent.h
 * Author: rbuck
 *
 * Created on November 19, 2010, 2:55 PM
 */

#ifndef _UDPRECEIVEPACKETEVENT_H
#define	_UDPRECEIVEPACKETEVENT_H

#include "PacketEvent.h"

class UDPReceivePacketEvent : public PacketEvent {
public:

    UDPReceivePacketEvent(int socket, Packet* packet) : PacketEvent(socket, packet) {

    }

    virtual ~UDPReceivePacketEvent() {

    }

    void execute(IModule* m) {
        m->udp_receive(this);
    }
};


#endif	/* _UDPRECEIVEPACKETEVENT_H */

