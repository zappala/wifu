/* 
 * File:   ReceiveBufferNotFullEvent.h
 * Author: rbuck
 *
 * Created on April 22, 2011, 1:01 PM
 */

#ifndef RECEIVEBUFFERNOTFULLEVENT_H
#define	RECEIVEBUFFERNOTFULLEVENT_H

#include "ProtocolEvent.h"
#include "Socket.h"
#include "IModule.h"

class ReceiveBufferNotFullEvent : public ProtocolEvent {
public:
    ReceiveBufferNotFullEvent(Socket* s);
    ~ReceiveBufferNotFullEvent();
    void execute(IModule* m);
};

#endif	/* RECEIVEBUFFERNOTFULLEVENT_H */
