/* 
 * File:   NetworkSendPacketEvent.h
 * Author: rbuck
 *
 * Created on January 28, 2011, 11:56 AM
 */

#ifndef _NETWORKSENDPACKETEVENT_H
#define	_NETWORKSENDPACKETEVENT_H

#include "PacketHolder.h"
#include "Socket.h"
#include "IModule.h"
#include "FrameworkEvent.h"

class NetworkSendPacketEvent : public PacketHolder, public FrameworkEvent {
public:
    NetworkSendPacketEvent(Socket* socket, WiFuPacket* packet);

    virtual ~NetworkSendPacketEvent();

    void execute(IModule* m);

};

#endif	/* _NETWORKSENDPACKETEVENT_H */
