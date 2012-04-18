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

using namespace std;

/**
 * Event which notfies a Protocol that a Socket and it's IContext objects need to be removed (probably because the connection is closed and/or the application called close()).
 *
 * @see Event
 * @see ProtocolEvent
 * @see Protocol
 */
class DeleteSocketEvent : public ProtocolEvent {
public:

    /**
     * Constructs a DeleteSocketEvent.s
     * @param s The Socket object to which this Event belongs and which is about to be removed with it's context.
     */
    DeleteSocketEvent(Socket* s);

    /**
     * Destructor.
     */
    virtual ~DeleteSocketEvent();

    /**
     * Calls IModule::imodule_delete_socket() and passes this DeleteSocketEvent in as the argument.
     *
     * @param m The module which to call IModule::imodule_delete_socket() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_delete_socket()
     */
    void execute(IModule* m);

};

#endif	/* DELETESOCKETEVENT_H */

