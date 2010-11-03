/* 
 * File:   TimerFiredEvent.h
 * Author: rbuck
 *
 * Created on November 1, 2010, 3:09 PM
 */

#ifndef _TIMERFIREDEVENT_H
#define	_TIMERFIREDEVENT_H

#include "Event.h"
#include "TimeoutEvent.h"


class TimerFiredEvent : public Event {
public:
    TimerFiredEvent(int socket, TimeoutEvent* timeout_event) : Event(socket), timeout_event_(timeout_event) {

    }

    virtual ~TimerFiredEvent() {

    }

    TimeoutEvent* get_timeout_event() {
        return timeout_event_;
    }

    void execute(IModule* m) {
        m->timer_fired(this);
    }

private:
    TimeoutEvent* timeout_event_;
};

#endif	/* _TIMERFIREDEVENT_H */

