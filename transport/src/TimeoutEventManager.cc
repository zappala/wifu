/*
 * File:   TimeoutEventManager.h
 * Author: rbuck
 *
 * Created on October 21, 2010, 12:41 PM
 */

#include "TimeoutEventManager.h"

TimeoutEventManager::TimeoutEventManager() : Module(new PriorityQueue<Event*, TimeoutEventComparator>()) {

    //make sure singleton is created
    TimeoutManagerSemaphore;

    signal(SIG_ENQUEUE_EVENT, signal_manager);
}

TimeoutEventManager& TimeoutEventManager::instance() {
    static TimeoutEventManager instance_;
    return instance_;
}

TimeoutEventManager::~TimeoutEventManager() {

}

void TimeoutEventManager::imodule_cancel_timer(Event * e) {
    CancelTimerEvent* event = (CancelTimerEvent*) e;
    canceled_events_.insert(event->get_timeout_event());

    // We must post in case the event which was canceled was the one we were waiting on.
    TimeoutManagerSemaphore.post();
}

void TimeoutEventManager::imodule_timeout(Event* e) {
    TimeoutEvent* event = (TimeoutEvent*) e;

    if (canceled_events_.contains(event)) {
        canceled_events_.remove(event);
        return;
    }

    bool timedout = TimeoutManagerSemaphore.timed_wait(&event->get_timeout_time());

    if (timedout) {
        Dispatcher::instance().enqueue(new TimerFiredEvent(event));
        return;
    }

    // Semaphore was posted on
    this->QueueProcessor<Event*>::enqueue(e, false);
}

void TimeoutEventManager::enqueue(Event* e, bool signal) {
    // TODO: This is cheating... (but currently (as of Feb. 16, 2011) the best thought of solution)
    // We are not enqueuing it to our queue, we are taking it before
    // so the CancelTimerEvent doesn't get stuck behind any
    // TimeoutEvents
    if(!strcmp(type_name(*e), type_name(CancelTimerEvent))){
        imodule_cancel_timer(e);
        return;
    }

    this->QueueProcessor<Event*>::enqueue(e, true);
}

void signal_manager(int signal) {
    switch (signal) {
        case SIG_ENQUEUE_EVENT:
            TimeoutManagerSemaphore.post();
            assert(TimeoutManagerSemaphore.get_value() <= 1);
            break;
    }
}