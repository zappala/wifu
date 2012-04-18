/* 
 * File:   ProtocolEvent.h
 * Author: rbuck
 *
 * Created on June 1, 2011, 1:42 PM
 */

#ifndef PROTOCOLEVENT_H
#define	PROTOCOLEVENT_H

#include "events/Event.h"
#include "defines.h"

/**
 * Represents an Event with a HIGH priority.
 * Certain Protocols may need to ensure that they process all ProtocolEvents before processing another FrameworkEvent.
 * Events such as an application request and receiving packets are of lower priority than receiving information about a buffer (being empty or full).
 * ProtocolEvent objects should not be dispatched to the dispatcher; rather, they should be directly enqueued to a specific QueueProcessor.
 *
 * @see Event
 * @see ProtocolEvent
 * @see Priority
 */
class ProtocolEvent : public Event {
public:

    /**
     * Constructs a HIGH Priority Event.
     * @param s The socket to which to apply this Event to.
     *
     * @see Event
     * @see Priority
     */
    ProtocolEvent(Socket* s);

    /**
     * Destructor.
     */
    virtual ~ProtocolEvent();
};

#endif	/* PROTOCOLEVENT_H */

