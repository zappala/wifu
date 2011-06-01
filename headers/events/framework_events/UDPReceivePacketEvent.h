/* 
 * File:   UDPReceivePacketEvent.h
 * Author: rbuck
 *
 * Created on November 19, 2010, 2:55 PM
 */

#ifndef _UDPRECEIVEPACKETEVENT_H
#define	_UDPRECEIVEPACKETEVENT_H

#include "events/PacketEvent.h"
#include "Socket.h"

class UDPReceivePacketEvent : public PacketEvent {
public:
    UDPReceivePacketEvent(Socket* socket, WiFuPacket* packet);

    virtual ~UDPReceivePacketEvent();

    void execute(IModule* m);

};

#endif	/* _UDPRECEIVEPACKETEVENT_H */

