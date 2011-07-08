/* 
 * File:   TimeoutEvent.h
 * Author: rbuck
 *
 * Created on October 21, 2010, 12:51 PM
 */

#ifndef _TIMEOUTEVENT_H
#define	_TIMEOUTEVENT_H

#include "FrameworkEvent.h"
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
 *
 * @see Event
 * @see TimeoutManager
 * @see CancelTimerEvent
 * @see TimerFiredEvent
 */
class TimeoutEvent : public FrameworkEvent {
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
     * Calls IModule::imodule_timeout() and passes this TimeoutEvent in as the argument.
     *
     * @param m The module which to call IModule::imodule_timeout() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_timeout()
     */
    void execute(IModule* m);

    /**
     * @return The string representation of the exact time at which this timer is supposed to go off.
     */
    gcstring to_s();

private:
    
    /**
     * Holds the absolute time when this TimeoutEvent should time out.
     */
    struct timespec timer_;

};


#endif	/* _TIMEOUTEVENT_H */
