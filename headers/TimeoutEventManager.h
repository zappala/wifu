/* 
 * File:   DeltaClock.h
 * Author: rbuck
 *
 * Created on October 21, 2010, 12:41 PM
 */

#ifndef _TIMEOUTEVENTMANAGER_H
#define	_TIMEOUTEVENTMANAGER_H

#include "PriorityQueue.h"
#include "TimeoutEvent.h"
#include "defines.h"
#include "TimeoutEventManagerSemaphore.h"
#include "CanceledEvents.h"
#include "Module.h"
#include "CancelTimerEvent.h"

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <errno.h>

#define TimeoutManagerSemaphore TimeoutEventManagerSemaphore::instance()


using namespace std;

/**
 * Handles registered signals.
 * The method is called when certain signals are raised.
 *
 * @param signal The signal which was raised triggering this method call.
 */
void signal_manager(int signal);

/**
 * Manages all TimeoutEvent objects.
 * Receives all TimeoutEvent objects via its enqueue() method;
 * when the absolute time in each TimeoutEvent occurs a TimerFiredEvent is enqueued to the Dispatcher.
 */
class TimeoutEventManager : public Module {
public:

    /**
     * Creates a TimeoutEventManager object.
     */
    TimeoutEventManager();

    /**
     * Cleans up this TimeoutEventManager object.
     */
    virtual ~TimeoutEventManager();

    /**
     * Enqueues e into the internal queue which holds all (Timeout)Event objects.
     *
     * @param e The Event to enqueue.
     * @param signal If true, will cause the internal queue to raise a SIG_ENQUEUE_EVENT signal.  This parameter is best left untouched by developers.
     * @see QueueProcessor::enqueue()
     */
    virtual void enqueue(Event* e, bool signal = true);

    /**
     * This method is a callback used when a timeout occurs.
     *
     * @param e The (Timeout)Event which timed out.
     */
    virtual void timeout(Event* e);

    /**
     * Prevent an Event from timing out.
     *
     * @param e The Event which we no longer wish to time out.
     */
    virtual void cancel_timer(Event* e);


private:

    /**
     * PriorityQueue passed to the parent QueueProcessor object.
     */
    PriorityQueue<Event*, EventComparator> queue_;

};



#endif	/* _TIMEOUTEVENTMANAGER_H */

