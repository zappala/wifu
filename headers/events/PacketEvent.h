/* 
 * File:   PacketEvent.h
 * Author: rbuck
 *
 * Created on November 19, 2010, 2:48 PM
 */

#ifndef _PACKETEVENT_H
#define	_PACKETEVENT_H

#include "Event.h"

class PacketEvent : public Event {
public:
    PacketEvent(int socket, Packet* p) : Event(socket) {
        
    }
    
    virtual ~PacketEvent() {
        delete packet_;
    }
    
    Packet* get_packet() {
        return packet_;
    }

private:
    Packet* packet_;
};

#endif	/* _PACKETEVENT_H */

