/* 
 * File:   CancelTimerEvent.h
 * Author: rbuck
 *
 * Created on November 3, 2010, 2:56 PM
 */

#ifndef _CANCELTIMEREVENT_H
#define	_CANCELTIMEREVENT_H

#include "TimerEvent.h"

/**
 * Cancels a TimeoutEvent.
 * Users should note that due to the event-driven nature of WiFu it is possilble to cancel a timer and still receive it.
 * Users should still check to see whether or not the timer they received is the one they expected.
 *
 * @see TimerEvent
 * @see TimeoutEvent
 * @see Event
 * @see TimeoutManager
 */
class CancelTimerEvent : public TimerEvent {
public:
    /**
     * Creates a CancelTimerEvent.
     *
     * @param event The TimeoutEvent which this CancelTimerEvent is to cancel.
     */
    CancelTimerEvent(TimeoutEvent* event);

    /**
     * Cleans up this CancelTimerEvent.
     */
    virtual ~CancelTimerEvent();

    /**
     * Calls IModule::imodule_cancel_timer() and passes this CancelTimerEvent in as the argument.
     *
     * @param m The module which to call IModule::imodule_cancel_timer() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_cancel_timer()
     */
    void execute(IModule* m);

};

#endif	/* _CANCELTIMEREVENT_H */
