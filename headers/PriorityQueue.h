/* 
 * File:   Queue.h
 * Author: rbuck
 *
 * Created on October 21, 2010, 6:41 PM
 */

#ifndef _PRIORITYQUEUE_H
#define	_PRIORITYQUEUE_H


#include <queue>
#include <semaphore.h>
#include <typeinfo>
#include <signal.h>
#include "includes.h"
#include "TimeoutEvent.h"

using namespace std;


// do we want to make this like a python Queue object?  Only a few more functions to go...

template<class T, class Comparator>
class PriorityQueue {
public:

    PriorityQueue() {
        sem_init(&sem_, 0, 1);
        sem_init(&counter_, 0, 0);
    }

    ~PriorityQueue() {

    }

    // do we want to do a timed wait? if so use: sem_timedwait() see the sem_wait man page

    T dequeue() {
        sem_wait(&counter_);
        sem_wait(&sem_);
        T value = q_.top();
        q_.pop();
        sem_post(&sem_);
        return value;
    }

    void enqueue(T obj, bool signal = true) {
        sem_wait(&sem_);
        q_.push(obj);
        if (signal) {
            raise(SIGENQUEUE);
         }
        sem_post(&sem_);
        sem_post(&counter_);
    }

    int size() {
        sem_wait(&sem_);
        int value = q_.size();
        sem_post(&sem_);
        return value;
    }

    bool isEmpty() {
        // protection is done in size()
        return size() == 0;
    }

private:
    sem_t sem_;
    sem_t counter_;

    priority_queue<T, deque<T>, Comparator> q_;
};


#endif	/* _PRIORITYQUEUE_H */

