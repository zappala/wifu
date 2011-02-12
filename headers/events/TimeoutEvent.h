/* 
 * File:   TimeoutEvent.h
 * Author: rbuck
 *
 * Created on October 21, 2010, 12:51 PM
 */

#ifndef _TIMEOUTEVENT_H
#define	_TIMEOUTEVENT_H

#include "Event.h"
#include "IModule.h"
#include "Utils.h"

#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include "Socket.h"


using namespace std;

/**
 * Event which represents a timeout at some point in the future.
 */
class TimeoutEvent : public Event {
public:

    /**
     * Constructs a TimeoutEvent.
     * The timeout time will be set to the time this object is created plus seconds and nanoseconds.
     *
     * @param socket The socket to which this TimeoutEvent will be associated.
     * @param seconds The number of seconds in the future in which to timeout.
     * @param nanoseconds The number nanoseconds in the future in which to timeout.
     */
    TimeoutEvent(Socket* socket, int seconds, long int nanoseconds) : Event(socket) {
        Utils::get_timespec_future_time(seconds, nanoseconds, &timer_);
    }

    /**
     * @return A reference to the timespec holding the absolute time this TimeoutEvent is due to timeout.
     */
    struct timespec & get_timeout_time() {
        return timer_;
    }

    /**
     * Determines if this TimeoutEvent's absolute timeout time is earlier than e's.
     *
     * @param e The (Timeout)Event to compare this TimeoutEvent to.
     * @return True if this TimeoutEvent is less than e's, false otherwise.
     */
    virtual bool less_than(Event* e) {
        TimeoutEvent* rhs = (TimeoutEvent*) e;

        struct timespec * a = &(get_timeout_time());
        struct timespec * b = &(rhs->get_timeout_time());

        if (a->tv_sec > b->tv_sec) return true;
        if (a->tv_sec == b->tv_sec && a->tv_nsec > b->tv_nsec) return true;
        return false;
    }

    /**
     * Will call timeout() on m.
     *
     * @param m The IModule which to call timeout() on.
     * @see IModule::timeout()
     */
    void execute(IModule* m) {
        m->timeout(this);
    }


private:
    /**
     * Holds the absolute time when this TimeoutEvent should time out.
     */
    struct timespec timer_;

};



#endif	/* _TIMEOUTEVENT_H */

