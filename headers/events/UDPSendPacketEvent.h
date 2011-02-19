/* 
 * File:   UDPSendPacketEvent.h
 * Author: rbuck
 *
 * Created on November 19, 2010, 2:52 PM
 */

#ifndef _UDPSENDPACKETEVENT_H
#define	_UDPSENDPACKETEVENT_H

#include "PacketEvent.h"
#include "Socket.h"

class UDPSendPacketEvent : public PacketEvent {
public:
    UDPSendPacketEvent(Socket*, WiFuPacket*);

    virtual ~UDPSendPacketEvent();
    
    void execute(IModule*);

};

#endif	/* _UDPSENDPACKETEVENT_H */

