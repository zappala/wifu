/* 
 * File:   ProtocolEvent.h
 * Author: rbuck
 *
 * Created on June 1, 2011, 1:42 PM
 */

#ifndef PROTOCOLEVENT_H
#define	PROTOCOLEVENT_H

#include "events/Event.h"
#include "defines.h"

class ProtocolEvent : public Event {
public:
    ProtocolEvent(Socket* s);
    virtual ~ProtocolEvent();
};

#endif	/* PROTOCOLEVENT_H */

