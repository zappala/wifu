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

class TimerEvent : public Event {
public:
    public:
    TimerEvent(TimeoutEvent* timeout_event) : Event(timeout_event->get_socket()), timeout_event_(timeout_event) {

    }

    virtual ~TimerEvent() {

    }

    TimeoutEvent* get_timeout_event() {
        return timeout_event_;
    }

private:
    TimeoutEvent* timeout_event_;
};

#endif	/* _TIMEREVENT_H */

