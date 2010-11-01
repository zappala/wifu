/* 
 * File:   TimeoutEvent.h
 * Author: rbuck
 *
 * Created on October 21, 2010, 12:51 PM
 */

#ifndef _TIMEOUTEVENT_H
#define	_TIMEOUTEVENT_H

#include "Event.h"
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <iostream>



using namespace std;

class TimeoutEvent : public Event {
public:

    TimeoutEvent(int socket, int seconds, long int nanoseconds) : Event(socket) {

        assert(seconds >= 0);
        assert(nanoseconds >= 0);

        // Can we get better precision with real-time clock?
        // Needs to be value from epoch (not relative time)
        clock_gettime(CLOCK_REALTIME, &timer_);

        timer_.tv_nsec += nanoseconds;
        timer_.tv_sec += seconds;

        while (timer_.tv_nsec >= NANOSECONDS_IN_SECONDS) {
            timer_.tv_sec += 1;
            timer_.tv_nsec -= NANOSECONDS_IN_SECONDS;
        }
    }

    struct timespec & get_timeout_time() {
        return timer_;
    }

    virtual bool less_than(Event* e) {
        TimeoutEvent* rhs = (TimeoutEvent*) e;
        
        struct timespec * a = &(get_timeout_time());
        struct timespec * b = &(rhs->get_timeout_time());

        if (a->tv_sec > b->tv_sec) return true;
        if (a->tv_sec == b->tv_sec && a->tv_nsec > b->tv_nsec) return true;
        return false;
    }

    void execute(IModule* m) {
        cout << "In execute seconds " << get_timeout_time().tv_sec << endl;
        cout << "In execute nanoseconds " << get_timeout_time().tv_nsec << endl;
        cout << "ID: " << get_id() << endl;
    }

private:
    int timeout_;
    struct timespec timer_;
};



#endif	/* _TIMEOUTEVENT_H */

