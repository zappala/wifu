/* 
 * File:   SendBufferNotEmpty.h
 * Author: rbuck
 *
 * Created on April 6, 2011, 10:54 AM
 */

#ifndef SENDBUFFERNOTEMPTYEVENT_H
#define	SENDBUFFERNOTEMPTYEVENT_H

#include "ProtocolEvent.h"

using namespace std;

/**
 * Event which notifies that a specific Socket's send buffer is not empty.
 * This is to be used as notification that data is available to be sent over the socket.
 *
 * @see Event
 * @see ProtocolEvent
 * @see Socket
 */
class SendBufferNotEmptyEvent : public ProtocolEvent {
public:

    /**
     * Constructs a SendBufferNotEmptyEvent.
     * @param s The Socket object to which this Event belongs and whose send buffer is not empty.
     */
    SendBufferNotEmptyEvent(Socket* s);

    /**
     * Calls IModule::imodule_send_buffer_not_empty() and passes this SendBufferNotEmptyEvent in as the argument.
     *
     * @param m The module which to call IModule::imodule_send_buffer_not_empty() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_send_buffer_not_empty()
     */
    void execute(IModule* m);
};

#endif	/* SENDBUFFERNOTEMPTYEVENT_H */

