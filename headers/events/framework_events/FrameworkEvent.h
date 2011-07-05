/* 
 * File:   FrameworkEvent.h
 * Author: rbuck
 *
 * Created on June 1, 2011, 1:44 PM
 */

#ifndef FRAMEWORKEVENT_H
#define	FRAMEWORKEVENT_H

#include "events/Event.h"
#include "defines.h"

/**
 * Represents an Event with a LOW priority.
 * Certain Protocols may need to ensure that they process all ProtocolEvent's before processing another FrameworkEvent.
 * Events such as an application request and receiving packets are of lower priority than receiving information about a buffer (being empty or full).
 * 
 * @see Event
 * @see ProtocolEvent
 * @see Priority
 */
class FrameworkEvent : public Event {
public:

    /**
     * Constructs an Event with a LOW Priority.
     * @param s The socket to which to apply this Event to.
     *
     * @see Event
     * @see Priority
     */
    FrameworkEvent(Socket* s);

    /**
     * Destructor.
     */
    virtual ~FrameworkEvent();
};

#endif	/* FRAMEWORKEVENT_H */

