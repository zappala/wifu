/* 
 * File:   PacketEvent.h
 * Author: rbuck
 *
 * Created on November 19, 2010, 2:48 PM
 */

#ifndef _PACKETEVENT_H
#define	_PACKETEVENT_H

#include "Event.h"
#include "packet/WiFuPacket.h"
#include "Socket.h"

class PacketEvent : public Event {
public:
    PacketEvent(Socket*, WiFuPacket*);
    
    virtual ~PacketEvent();
    
    WiFuPacket* get_packet();

private:
    WiFuPacket* packet_;

};

#endif	/* _PACKETEVENT_H */
