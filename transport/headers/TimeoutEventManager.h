/* 
 * File:   TimeoutEventManager.h
 * Author: rbuck
 *
 * Created on October 21, 2010, 12:41 PM
 */

#ifndef _TIMEOUTEVENTMANAGER_H
#define	_TIMEOUTEVENTMANAGER_H

#include "PriorityQueue.h"
#include "events/framework_events/TimeoutEvent.h"
#include "defines.h"
#include "TimeoutEventManagerSemaphore.h"
#include "Module.h"
#include "events/framework_events/CancelTimerEvent.h"
#include "TimeoutEventComparator.h"

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
     * Accesses the one instance of this object
     */
    static TimeoutEventManager& instance();

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
    virtual void imodule_timeout(Event* e);

    /**
     * Prevent an Event from timing out.
     *
     * @param e The Event which we no longer wish to time out.
     */
    virtual void imodule_cancel_timer(Event* e);


private:

    /**
     * Creates a TimeoutEventManager object.
     */
    TimeoutEventManager();

    /**
     * TimeoutEvents which have been canceled
     */
    Set<TimeoutEvent*> canceled_events_;
};





#endif	/* _TIMEOUTEVENTMANAGER_H */

