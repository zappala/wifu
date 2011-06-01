/* 
 * File:   TimerEvent.h
 * Author: rbuck
 *
 * Created on November 3, 2010, 2:52 PM
 */

#ifndef _TIMEREVENT_H
#define	_TIMEREVENT_H

#include "FrameworkEvent.h"
#include "TimeoutEvent.h"

/**
 * Container class which simply holds a pointer to a TimeoutEvent.
 */
class TimerEvent : public FrameworkEvent {
public:
    /**
     * Constructs a TimerEvent.
     *
     * @param timeout_event The TimeoutEvent to store.
     */
    TimerEvent(TimeoutEvent* timeout_event);

    /**
     * Cleans up this TimerEvent object.
     */
    virtual ~TimerEvent();

    /**
     * @return A pointer to the TimeoutEvent stored internally to this TimerEvent.
     */
    TimeoutEvent* get_timeout_event();

private:
    /**
     * Pointer to an actual TimeoutEvent which this class stores.
     */
    TimeoutEvent* timeout_event_;

};

#endif	/* _TIMEREVENT_H */
