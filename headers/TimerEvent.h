/* 
 * File:   TimerEvent.h
 * Author: rbuck
 *
 * Created on November 3, 2010, 2:52 PM
 */

#ifndef _TIMEREVENT_H
#define	_TIMEREVENT_H

#include "IModule.h"
#include "Event.h"

/**
 * Container class which simply holds a pointer to a TimeoutEvent.
 */
class TimerEvent : public Event {
public:

    /**
     * Constructs a TimerEvent.
     *
     * @param timeout_event The TimeoutEvent to store.
     */
    TimerEvent(TimeoutEvent* timeout_event) : Event(timeout_event->get_socket()), timeout_event_(timeout_event) {

    }

    /**
     * Cleans up this TimerEvent object.
     */
    virtual ~TimerEvent() {

    }

    /**
     * @return A pointer to the TimeoutEvent stored internally to this TimerEvent.
     */
    TimeoutEvent* get_timeout_event() {
        return timeout_event_;
    }

private:
    TimeoutEvent* timeout_event_;
};

#endif	/* _TIMEREVENT_H */

