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
 * 
 * @see Event
 * @see TimerEvent
 * @see TimeoutEvent
 * @see CancelTimerEvent
 * @see TimeoutManager
 */
class TimerFiredEvent : public TimerEvent {
public:
    /**
     * Constructs a TimerFiredEvent object.
     *
     * @param timeout_event The TimeoutEvent which expired, causing the creation of this TimerFiredEvent.
     */
    TimerFiredEvent(TimeoutEvent* timeout_event);

    /**
     * Cleans up this TimerFiredEvent object.
     */
    virtual ~TimerFiredEvent();

    /**
     * Calls IModule::imodule_timer_fired() and passes this TimerFiredEvent in as the argument.
     * It is important that IModules understand that many timers will be going off and they need to keep track of the TimeoutEvent (pointer) they send and compare them with the one inside this TimerFiredEvent.
     *
     * @param m The module which to call IModule::imodule_timer_fired() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_timer_fired()
     */
    void execute(IModule* m);

};

#endif	/* _TIMERFIREDEVENT_H */
