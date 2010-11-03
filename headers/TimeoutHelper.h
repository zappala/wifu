/* 
 * File:   TimeoutHelper.h
 * Author: rbuck
 *
 * Created on November 3, 2010, 2:04 PM
 */

#ifndef _TIMEOUTHELPER_H
#define	_TIMEOUTHELPER_H

#include "EventSet.h"
#include "TimeoutEvent.h"
#include "Dispatcher.h"
#include "TimerFiredEvent.h"
#include "CancelTimerEvent.h"

class TimeoutHelper : public EventSet {
public:
    TimeoutHelper() : EventSet() {

    }

    virtual ~ TimeoutHelper() {

    }

    void dispatch_timeout(Event* event) {
        add(event);
        Dispatcher::instance().enqueue(event);
    }

    bool is_my_timeout(TimerFiredEvent* event) {
        bool value = contains(event->get_timeout_event());
        remove(event);
        return value;
    }

    void cancel_timeout(TimeoutEvent* event) {
        remove(event);
        Dispatcher::instance().enqueue(new CancelTimerEvent(event));
    }
};

#endif	/* _TIMEOUTHELPER_H */

