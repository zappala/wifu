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

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <errno.h>

#define TimeoutManagerSemaphore TimeoutEventManagerSemaphore::instance()


using namespace std;

// prototypes
void * dequeue_thread(void * arg);
void signal_manager(int signal);

class TimeoutEventManager {
public:

    TimeoutEventManager();
    virtual ~TimeoutEventManager();
    void enqueue(TimeoutEvent * event);
    void cancel(TimeoutEvent * event);


private:
    PriorityQueue<TimeoutEvent *, TimeoutEventComparator> q_;
    pthread_t thread_;
};



#endif	/* _TIMEOUTEVENTMANAGER_H */

