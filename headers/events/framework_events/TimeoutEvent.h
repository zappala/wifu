/* 
 * File:   TimeoutEvent.h
 * Author: rbuck
 *
 * Created on October 21, 2010, 12:51 PM
 */

#ifndef _TIMEOUTEVENT_H
#define	_TIMEOUTEVENT_H

#include "events/Event.h"
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
    TimeoutEvent(Socket* socket, int seconds, long int nanoseconds);

    /**
     * @return A reference to the timespec holding the absolute time this TimeoutEvent is due to timeout.
     */
    struct timespec & get_timeout_time();

    /**
     * Will call timeout() on m.
     *
     * @param m The IModule which to call timeout() on.
     * @see IModule::timeout()
     */
    void execute(IModule* m);

    /**
     * @return The stirng representation of the exact time at which this timer is supposed to go off.
     */
    string to_s();

private:
    /**
     * Holds the absolute time when this TimeoutEvent should time out.
     */
    struct timespec timer_;

};


#endif	/* _TIMEOUTEVENT_H */
