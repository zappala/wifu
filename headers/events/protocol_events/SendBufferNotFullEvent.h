/* 
 * File:   SendBufferNotFull.h
 * Author: rbuck
 *
 * Created on April 6, 2011, 10:54 AM
 */

#ifndef SENDBUFFERNOTFULLEVENT_H
#define	SENDBUFFERNOTFULLEVENT_H

#include "ProtocolEvent.h"

class SendBufferNotFullEvent : public ProtocolEvent {
public:
    SendBufferNotFullEvent(Socket* s);

    void execute(IModule* m);
};

#endif	/* SENDBUFFERNOTFULLEVENT_H */

