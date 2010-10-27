/*
 * File:   TimeoutEventManager.h
 * Author: rbuck
 *
 * Created on October 21, 2010, 12:41 PM
 */

#include "TimeoutEventManager.h"

TimeoutEventManager::TimeoutEventManager() {

    //make sure singleton is created
    TimeoutManagerSemaphore;

    if (pthread_create(&thread_, NULL, &dequeue_thread, &q_) != 0) {
        perror("Error creating new thread");
        exit(EXIT_FAILURE);
    }

    signal(SIG_ENQUEUE_EVENT, signal_manager);
    signal(SIG_CANCEL_EVENT, signal_manager);
}

TimeoutEventManager::~TimeoutEventManager() {

}

void TimeoutEventManager::enqueue(TimeoutEvent * event) {
    q_.enqueue(event);
}

void TimeoutEventManager::cancel(TimeoutEvent * event) {
    CanceledEvents::instance().add(event);
    raise(SIG_CANCEL_EVENT);
}

void * dequeue_thread(void* arg) {
    PriorityQueue<TimeoutEvent *, TimeoutEventComparator> * q =
            (PriorityQueue<TimeoutEvent *, TimeoutEventComparator> *) arg;

    while (1) {
        TimeoutEvent * event = q->dequeue();

        if(CanceledEvents::instance().is_canceled(event)) {
            CanceledEvents::instance().remove(event);
            delete event;
            continue;
        }

        bool timedout = TimeoutManagerSemaphore.timed_wait(&event->get_timeout_time());

        if (timedout) {
            event->execute();
            delete event;
            continue;
        }

        // Semaphore was posted on
        q->enqueue(event, false);
    }
}

void signal_manager(int signal) {
    switch (signal) {
        case SIG_ENQUEUE_EVENT:
        case SIG_CANCEL_EVENT:
            TimeoutManagerSemaphore.post();
            break;
    }
}




