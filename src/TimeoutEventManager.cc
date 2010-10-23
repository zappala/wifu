/*
 * File:   TimeoutEventManager.h
 * Author: rbuck
 *
 * Created on October 21, 2010, 12:41 PM
 */

#include "TimeoutEventManager.h"

TimeoutEventManager::TimeoutEventManager() {

    //make sure singleton is created
    TimeoutEventManagerSemaphore::instance();

    if (pthread_create(&thread_, NULL, &delta_clock_thread, &q_) != 0) {
        perror("Error creating new thread");
        exit(EXIT_FAILURE);
    }

    signal(SIGENQUEUE, signal_manager);
}

TimeoutEventManager::~TimeoutEventManager() {

}

void TimeoutEventManager::enqueue(TimeoutEvent * event) {
    q_.enqueue(event);
}

void TimeoutEventManager::cancel(TimeoutEvent* event) {

}

void * delta_clock_thread(void* arg) {
    PriorityQueue<TimeoutEvent *, TimeoutEventComparator> * q =
            (PriorityQueue<TimeoutEvent *, TimeoutEventComparator> *) arg;

    while (1) {
        TimeoutEvent * event = q->dequeue();
        TimeoutEventManagerSemaphore::instance().timed_wait(&event->get_timeout_time());
        if (errno == ETIMEDOUT) {
            // we timed out
            errno = 0;
            event->execute();
            continue;
        }
        // tosem was posted on
        q->enqueue(event, false);
    }
}

void signal_manager(int signal) {
    switch (signal) {
        case SIGENQUEUE:
            TimeoutEventManagerSemaphore::instance().post();
    }
}




