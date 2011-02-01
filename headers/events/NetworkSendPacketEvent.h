/* 
 * File:   NetworkSendPacketEvent.h
 * Author: rbuck
 *
 * Created on January 28, 2011, 11:56 AM
 */

#ifndef _NETWORKSENDPACKETEVENT_H
#define	_NETWORKSENDPACKETEVENT_H

#include "PacketEvent.h"
#include "../Socket.h"

class NetworkSendPacketEvent : public PacketEvent {
public:
    NetworkSendPacketEvent(Socket* socket, WiFuPacket* packet) : PacketEvent(socket, packet) {

    }

    virtual ~NetworkSendPacketEvent() {

    }

    void execute(IModule* m) {
        m->network_send(this);
    }
};

#endif	/* _NETWORKSENDPACKETEVENT_H */

