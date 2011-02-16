/*
 * File:   TimeoutEventManager.h
 * Author: rbuck
 *
 * Created on October 21, 2010, 12:41 PM
 */

#include "TimeoutEventManager.h"

TimeoutEventManager::TimeoutEventManager() : Module(new PriorityQueue<Event*, EventComparator>()) {

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

void TimeoutEventManager::cancel_timer(Event * e) {
//    cout << "Cancelling Timer" << endl;
    CancelTimerEvent* event = (CancelTimerEvent*) e;
    canceled_events_.insert(event->get_timeout_event());
}

void TimeoutEventManager::timeout(Event* e) {
//    cout << "In timeout() in TimeoutEventManager\n";
    TimeoutEvent* event = (TimeoutEvent*) e;

    if (canceled_events_.contains(event)) {
        canceled_events_.remove(event);
        return;
    }

    bool timedout = TimeoutManagerSemaphore.timed_wait(&event->get_timeout_time());

    if (timedout) {
//        cout << "Timed out!\n";
        Dispatcher::instance().enqueue(new TimerFiredEvent(event));
        return;
    }

    // Semaphore was posted on
    enqueue(event, false);
}

void TimeoutEventManager::enqueue(Event* e, bool signal) {
//    cout << "We're enqueuing in TimeoutEventManager.\n";
    
    // TODO: This is cheating... (but currently (as of Feb. 16, 2011) the best thought of solution)
    // We are not enqueuing it to our queue, we are taking it before
    // so the CancelTimerEvent doesn't get stuck behind any
    // TimeoutEvents
    if(type_name(*e) == type_name(CancelTimerEvent)) {
        cancel_timer(e);
        return;
    }

    this->QueueProcessor<Event*>::enqueue(e, signal);

}

void signal_manager(int signal) {
    switch (signal) {
        case SIG_ENQUEUE_EVENT:
            TimeoutManagerSemaphore.post();
            break;
    }
}