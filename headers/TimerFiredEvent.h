/* 
 * File:   TimerFiredEvent.h
 * Author: rbuck
 *
 * Created on November 1, 2010, 3:09 PM
 */

#ifndef _TIMERFIREDEVENT_H
#define	_TIMERFIREDEVENT_H

#include "TimerEvent.h"

class TimerFiredEvent : public TimerEvent {
public:

    TimerFiredEvent(TimeoutEvent* timeout_event) : TimerEvent(timeout_event) {

    }

    virtual ~TimerFiredEvent() {

    }

    void execute(IModule* m) {
        m->timer_fired(this);
    }
};

#endif	/* _TIMERFIREDEVENT_H */

