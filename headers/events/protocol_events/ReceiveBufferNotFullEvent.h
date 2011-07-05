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

/**
 * Event which notifies that a specific Socket's receive buffer is not full.
 *
 * @see Event
 * @see ProtocolEvent
 * @see Socket
 */
class ReceiveBufferNotFullEvent : public ProtocolEvent {
public:

    /**
     * Constructs a ReceiveBufferNotFullEvent
     * @param s The Socket object to which this Event belongs and whose receive buffer is not full.
     */
    ReceiveBufferNotFullEvent(Socket* s);

    /**
     * Destructor.
     */
    virtual ~ReceiveBufferNotFullEvent();

    /**
     * Calls IModule::imodule_receive_buffer_not_full() and passes this ReceiveBufferNotFullEvent in as the argument.
     *
     * @param m The module which to call IModule::imodule_receive_buffer_not_full() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_receive_buffer_not_full()
     */
    void execute(IModule* m);
};

#endif	/* RECEIVEBUFFERNOTFULLEVENT_H */

