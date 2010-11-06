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

/**
 * Class wich aides a Module in filtering TimerFiredEvent objects.
 *
 * Due to the fact that every Module will receive all TimerFiredEvent objects,
 * this class helps filter and pass along only those which a corresponding TimeoutEvent was created by this class.
 */
class TimeoutHelper : public EventSet {
public:

    /**
     * Creates a TimeoutHelper object.
     */
    TimeoutHelper() : EventSet() {

    }

    /**
     * Cleans up this TimeoutHelper object.
     */
    virtual ~ TimeoutHelper() {

    }

    /**
     * Keeps track of event as coming from this object and then calls Dispacther::enqueue()
     *
     * @param event The Event to keep track of.
     */
    void dispatch_timeout(Event* event) {
        add(event);
        Dispatcher::instance().enqueue(event);
    }

    /**
     * Determines if the TimeoutEvent internal to event was sent via this object's dispatch_timeout() method.
     * Calling this method twice in a row on the same TimerFiredEvent will always result in the second call returning false.
     *
     * @param event The TimerFiredEvent in question as to whether this object generated its TimeoutEvent.
     * @return True if the TimeoutEvent internal to event was sent via this object's dispatch_timeout() method, false otherwise.
     */
    bool is_my_timeout(TimerFiredEvent* event) {
        bool value = contains(event->get_timeout_event());
        remove(event);
        return value;
    }

    /**
     * Generates a CancelTimerEvent and sends it to the Dispatcher.
     *
     * @param event The TimeoutEvent to cancel.
     */
    void cancel_timeout(TimeoutEvent* event) {
        remove(event);
        Dispatcher::instance().enqueue(new CancelTimerEvent(event));
    }
};

#endif	/* _TIMEOUTHELPER_H */

