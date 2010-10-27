/*
 * File:   TimeoutEventManager.h
 * Author: rbuck
 *
 * Created on October 21, 2010, 12:41 PM
 */

#include "TimeoutEventManager.h"

TimeoutEventManager::TimeoutEventManager() 
 : QueueProcessor<TimeoutEvent*>(&queue_, this) {

    //make sure singleton is created
    TimeoutManagerSemaphore;

    signal(SIG_ENQUEUE_EVENT, signal_manager);
    signal(SIG_CANCEL_EVENT, signal_manager);
}

TimeoutEventManager::~TimeoutEventManager() {

}

void TimeoutEventManager::cancel(TimeoutEvent * event) {
    CanceledEvents::instance().add(event);
    raise(SIG_CANCEL_EVENT);
}

void TimeoutEventManager::process(TimeoutEvent* event) {
    if (CanceledEvents::instance().is_canceled(event)) {
        CanceledEvents::instance().remove(event);
        delete event;
        return;
    }

    bool timedout = TimeoutManagerSemaphore.timed_wait(&event->get_timeout_time());

    if (timedout) {
        event->execute();
        delete event;
        return;
    }

    // Semaphore was posted on
    enqueue(event, false);
}

void signal_manager(int signal) {
    switch (signal) {
        case SIG_ENQUEUE_EVENT:
        case SIG_CANCEL_EVENT:
            TimeoutManagerSemaphore.post();
            break;
    }
}