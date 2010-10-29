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

    TimeoutEvent(int seconds, long int nanoseconds) : Event() {

        assert(seconds >= 0);
        assert(nanoseconds >= 0);

        // Can we get better precision with real-time clock?
        // Needs to be value from epoch (not relative time)
        clock_gettime(CLOCK_REALTIME, &timer_);

        timer_.tv_nsec += nanoseconds;
        timer_.tv_sec += seconds;

        while(timer_.tv_nsec >= NANOSECONDS_IN_SECONDS) {
            timer_.tv_sec += 1;
            timer_.tv_nsec -= NANOSECONDS_IN_SECONDS;
        }
    }

    struct timespec & get_timeout_time() {
        return timer_;
    }

    void execute(IQModule* m) {
        cout << "In execute seconds " << get_timeout_time().tv_sec << endl;
        cout << "In execute nanoseconds " << get_timeout_time().tv_nsec << endl;
        cout << "ID: " << get_id() << endl;
    }

private:
    int timeout_;
    struct timespec timer_;
};

class TimeoutEventComparator {
public:

    bool operator()(TimeoutEvent*& t1, TimeoutEvent*& t2) {

        struct timespec * a = &(t1->get_timeout_time());
        struct timespec * b = &(t2->get_timeout_time());

        if (a->tv_sec > b->tv_sec) return true;
        if (a->tv_sec == b->tv_sec && a->tv_nsec > b->tv_nsec) return true;
        return false;
    } 
};

#endif	/* _TIMEOUTEVENT_H */

