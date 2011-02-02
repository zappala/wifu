/* 
 * File:   UDPReceivePacketEvent.h
 * Author: rbuck
 *
 * Created on November 19, 2010, 2:55 PM
 */

#ifndef _UDPRECEIVEPACKETEVENT_H
#define	_UDPRECEIVEPACKETEVENT_H

#include "PacketEvent.h"
#include "Socket.h"

class UDPReceivePacketEvent : public PacketEvent {
public:

    UDPReceivePacketEvent(Socket* socket, WiFuPacket* packet) : PacketEvent(socket, packet) {

    }

    virtual ~UDPReceivePacketEvent() {

    }

    void execute(IModule* m) {
        m->udp_receive(this);
    }
};


#endif	/* _UDPRECEIVEPACKETEVENT_H */

