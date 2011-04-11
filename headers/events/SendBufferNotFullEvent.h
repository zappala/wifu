/* 
 * File:   SendBufferNotFull.h
 * Author: rbuck
 *
 * Created on April 6, 2011, 10:54 AM
 */

#ifndef SENDBUFFERNOTFULLEVENT_H
#define	SENDBUFFERNOTFULLEVENT_H

#include "Event.h"

class SendBufferNotFullEvent : public Event {
public:
    SendBufferNotFullEvent(Socket* s);

    void execute(IModule* m);
};

#endif	/* SENDBUFFERNOTFULLEVENT_H */

