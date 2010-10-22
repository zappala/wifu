/* 
 * File:   DeltaClock.h
 * Author: rbuck
 *
 * Created on October 21, 2010, 12:41 PM
 */

#ifndef _DELTACLOCK_H
#define	_DELTACLOCK_H

#include "PriorityQueue.h"
#include "TimeoutEvent.h"
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

using namespace std;

sem_t sem;

// prototypes
void * delta_clock_thread(void * arg);
void signal_manager(int signal);

class DeltaClock {
public:

    DeltaClock() {
        sem_init(&sem, 0, 0);
        
        if (pthread_create(&thread_, NULL, &delta_clock_thread, &q_) != 0) {
            perror("Error creating new thread");
            exit(EXIT_FAILURE);
        }

        signal(SIGENQUEUE, signal_manager);
    }

    virtual ~DeltaClock() {

    }

    void enqueue(TimeoutEvent * event) {
        q_.enqueue(event);
    }

private:
    PriorityQueue<TimeoutEvent *, TimeoutEventComparator> q_;
    pthread_t thread_;

};

void * delta_clock_thread(void* arg) {
    PriorityQueue<TimeoutEvent *, TimeoutEventComparator> * q =
            (PriorityQueue<TimeoutEvent *, TimeoutEventComparator> *) arg;

    while (1) {
        TimeoutEvent * event = q->dequeue();
        sem_timedwait(&sem, &event->get_timeout_time());

        if (errno == ETIMEDOUT) {
            // we timed out
            errno = 0;
            event->execute();
            continue;
        }
        // sem was posted on
        q->enqueue(event, false);
    }
}

void signal_manager(int signal) {
    switch(signal) {
        case SIGENQUEUE:
            sem_post(&sem);
    }
}


#endif	/* _DELTACLOCK_H */

