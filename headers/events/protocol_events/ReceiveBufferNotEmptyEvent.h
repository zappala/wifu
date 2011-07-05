/* 
 * File:   ReceiveBufferNotEmpty.h
 * Author: rbuck
 *
 * Created on April 6, 2011, 10:54 AM
 */

#ifndef RECEIVEBUFFERNOTEMPTYEVENT_H
#define	RECEIVEBUFFERNOTEMPTYEVENT_H

#include "ProtocolEvent.h"

using namespace std;

/**
 * Event which notifies that a specific Socket's receive buffer is not empty.
 * This is to be used as notification that data is available to be passed to the application.
 *
 * @see Event
 * @see ProtocolEvent
 * @see Socket
 */
class ReceiveBufferNotEmptyEvent : public ProtocolEvent {
public:

    /**
     * Constructs a ReceiveBufferNotEmptyEvent
     * @param s The Socket object to which this Event belongs and whose receive buffer is not empty.
     */
    ReceiveBufferNotEmptyEvent(Socket* s);

    /**
     * Destructor.
     */
    virtual ~ReceiveBufferNotEmptyEvent();

    /**
     * Calls IModule::imodule_receive_buffer_not_empty() and passes this ReceiveBufferNotEmptyEvent in as the argument.
     *
     * @param m The module which to call IModule::imodule_receive_buffer_not_empty() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_receive_buffer_not_empty()
     */
    void execute(IModule* m);
};

#endif	/* RECEIVEBUFFERNOTEMPTYEVENT_H */

