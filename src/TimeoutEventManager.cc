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

    signal(SIG_ENQUEUE, signal_manager);
    signal(SIG_CANCEL, signal_manager);
}

TimeoutEventManager::~TimeoutEventManager() {

}

void TimeoutEventManager::enqueue(TimeoutEvent * event) {
    gc = 0;
    cout << "ENQUEUED" << endl;
    q_.enqueue(event);
}

void TimeoutEventManager::cancel(TimeoutEvent * event) {
    gc = 0;
    CanceledEvents::instance().add(event);
    raise(SIG_CANCEL);
}

void * dequeue_thread(void* arg) {
    PriorityQueue<TimeoutEvent *, TimeoutEventComparator> * q =
            (PriorityQueue<TimeoutEvent *, TimeoutEventComparator> *) arg;

    while (1) {
        TimeoutEvent * event = q->dequeue();

        cout << "DEQUEUED" << endl;

        if(CanceledEvents::instance().is_canceled(event)) {
            CanceledEvents::instance().remove(event);
            cout << "CANCELED" << endl;
            delete event;
            continue;
        }

        cout << "ABOUT TO WAIT: "<< TimeoutManagerSemaphore.get_value() << endl;
        TimeoutManagerSemaphore.timed_wait(&event->get_timeout_time());

        //TODO: Do we want to move the errno check inside the Semaphore class?
        if (errno == ETIMEDOUT) {
            // we timed out

            errno = 0;
            event->execute();
            //TODO: delete the event here?
            cout << "TIMEOUT" << endl;
            delete event;
            continue;
        }

        cout << "SEM POSTED ON, ENQUEUING" << endl;
        // Semaphore was posted on
        q->enqueue(event, false);
    }
}

void signal_manager(int signal) {
    switch (signal) {
        case SIG_ENQUEUE:
        case SIG_CANCEL:
            TimeoutManagerSemaphore.post();
            break;
    }
}




