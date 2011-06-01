/* 
 * File:   DeleteSocketEvent.h
 * Author: rbuck
 *
 * Created on April 21, 2011, 10:27 AM
 */

#ifndef DELETESOCKETEVENT_H
#define	DELETESOCKETEVENT_H

#include "ProtocolEvent.h"
#include "IModule.h"
#include "Socket.h"
#include "Dispatcher.h"

class DeleteSocketEvent : public ProtocolEvent {
public:
    DeleteSocketEvent(Socket* s);
    virtual ~DeleteSocketEvent();

    void execute(IModule* m);

};

#endif	/* DELETESOCKETEVENT_H */

