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
 * Event which represents the idea of canceling a timer.
 *
 * @see TimerEvent
 * @see Event
 */
class CancelTimerEvent : public TimerEvent {
public:
    /**
     * Creates a CancelTimerEvent.
     *
     * @param event The TimeoutEvent which this CancelTimerEvent is to cancel.
     */
    CancelTimerEvent(TimeoutEvent*);

    /**
     * Cleans up this CancelTimerEvent.
     */
    virtual ~CancelTimerEvent();

    /**
     * Will call cancel_timer(this) on m.
     *
     * @param m The module which to call cancel_timer() on.
     * @see IModule::cancel_timer()
     */
    void execute(IModule*);

};

#endif	/* _CANCELTIMEREVENT_H */
