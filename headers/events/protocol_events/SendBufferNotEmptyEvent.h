/* 
 * File:   SendBufferNotEmpty.h
 * Author: rbuck
 *
 * Created on April 6, 2011, 10:54 AM
 */

#ifndef SENDBUFFERNOTEMPTYEVENT_H
#define	SENDBUFFERNOTEMPTYEVENT_H

#include "ProtocolEvent.h"

class SendBufferNotEmptyEvent : public ProtocolEvent {
public:
    SendBufferNotEmptyEvent(Socket* s);

    void execute(IModule* m);
};

#endif	/* SENDBUFFERNOTEMPTYEVENT_H */

