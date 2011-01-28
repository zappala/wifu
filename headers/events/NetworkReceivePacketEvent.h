/* 
 * File:   NetworkReceivePacketEvent.h
 * Author: rbuck
 *
 * Created on January 28, 2011, 11:59 AM
 */

#ifndef _NETWORKRECEIVEPACKETEVENT_H
#define	_NETWORKRECEIVEPACKETEVENT_H

#include "PacketEvent.h"
#include "../Socket.h"

class NetworkReceivePacketEvent : public PacketEvent {
public:

    NetworkReceivePacketEvent(Socket* socket, WiFuPacket* packet) : PacketEvent(socket, packet) {

    }

    virtual ~NetworkReceivePacketEvent() {

    }

    void execute(IModule* m) {
        m->network_receive(this);
    }
};

#endif	/* _NETWORKRECEIVEPACKETEVENT_H */

