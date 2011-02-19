/* 
 * File:   TimerFiredEvent.h
 * Author: rbuck
 *
 * Created on November 1, 2010, 3:09 PM
 */

#ifndef _TIMERFIREDEVENT_H
#define	_TIMERFIREDEVENT_H

#include "TimerEvent.h"

/**
 * Event which represents the expiration of a TimeoutEvent.
 * @see TimeoutEvent
 */
class TimerFiredEvent : public TimerEvent {
public:
    /**
     * Constructs a TimerFiredEvent object.
     *
     * @param timeout_event The TimeoutEvent which expired, causing the creation of this TimerFiredEvent.
     */
    TimerFiredEvent(TimeoutEvent*);

    /**
     * Cleans up this TimerFiredEvent object.
     */
    virtual ~TimerFiredEvent();

    /**
     * Will call timer_fired() on m.
     *
     * @param m The IModule which to call timer_fired() on.
     * @see IModule::timer_fired()
     */
    void execute(IModule* m);

};

#endif	/* _TIMERFIREDEVENT_H */
