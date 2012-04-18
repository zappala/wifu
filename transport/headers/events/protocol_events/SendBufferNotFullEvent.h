/* 
 * File:   SendBufferNotFull.h
 * Author: rbuck
 *
 * Created on April 6, 2011, 10:54 AM
 */

#ifndef SENDBUFFERNOTFULLEVENT_H
#define	SENDBUFFERNOTFULLEVENT_H

#include "ProtocolEvent.h"

using namespace std;

/**
 * Event which notifies that a specific Socket's send buffer is not full.
 * This is to be used as notification that room is now available for more data from the application.
 *
 * @see Event
 * @see ProtocolEvent
 * @see Socket
 */
class SendBufferNotFullEvent : public ProtocolEvent {
public:

    /**
     * Constructs a SendBufferNotFullEvent.
     * @param s The Socket object to which this Event belongs and whose send buffer is not full.
     */
    SendBufferNotFullEvent(Socket* s);

    /**
     * Calls IModule::imodule_send_buffer_not_full() and passes this SendBufferNotEmptyEvent in as the argument.
     *
     * @param m The module which to call IModule::imodule_send_buffer_not_full() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_send_buffer_not_full()
     */
    void execute(IModule* m);
};

#endif	/* SENDBUFFERNOTFULLEVENT_H */

