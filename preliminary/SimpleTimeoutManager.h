/* 
 * File:   SimpleTimeoutManager.h
 * Author: rbuck
 *
 * Created on December 20, 2010, 10:23 AM
 */

#ifndef _SIMPLETIMEOUTMANAGER_H
#define	_SIMPLETIMEOUTMANAGER_H

#include <pthread.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <signal.h>

#define TIMEOUT 60

using namespace std;


void* timeout_thread(void* arg);
Semaphore start;
BinarySemaphore semaphore;
Semaphore flag;

class SimpleTimeoutManager {
public:

    SimpleTimeoutManager() : should_post_(false) {

        start.init(0);
        semaphore.init(0);
        flag.init(0);

        if (pthread_create(&timeout_thread_, NULL, timeout_thread, this)) {
            perror("Error creating new thread: SimpleTCP");
            exit(EXIT_FAILURE);
        }
        flag.post();
    }

    virtual ~SimpleTimeoutManager() {
        pthread_cancel(timeout_thread_);
    }

    void set_timer(int seconds, long int nanoseconds) {
        if(should_post_) {
            semaphore.post();
        }
        should_post_ = true;
        
        Utils::get_timespec_future_time(seconds, nanoseconds, get_timespec());
        start.post();
    }

    struct timespec* get_timespec() {
        return &ts_;
    }


private:
    pthread_t timeout_thread_;
    struct timespec ts_;
    bool should_post_;
};

void* timeout_thread(void* arg) {
    cout << "In SimpleTCP timeout thread" << endl;

    flag.wait();

    SimpleTimeoutManager* stm = (SimpleTimeoutManager*) arg;

    while (1) {
        start.wait();

        bool timedout = semaphore.timed_wait(stm->get_timespec());
        if(timedout) {
            raise(TIMEOUT);
        }
    }
}

#endif	/* _SIMPLETIMEOUTMANAGER_H */

