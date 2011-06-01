/* 
 * File:   NetworkReceivePacketEvent.h
 * Author: rbuck
 *
 * Created on January 28, 2011, 11:59 AM
 */

#ifndef _NETWORKRECEIVEPACKETEVENT_H
#define	_NETWORKRECEIVEPACKETEVENT_H

#include "PacketHolder.h"
#include "Socket.h"
#include "FrameworkEvent.h"

class NetworkReceivePacketEvent : public PacketHolder, public FrameworkEvent {
public:
    NetworkReceivePacketEvent(Socket* socket, WiFuPacket* packet);

    virtual ~NetworkReceivePacketEvent();

    void execute(IModule* m);

};

#endif	/* _NETWORKRECEIVEPACKETEVENT_H */
