/* 
 * File:   Queue.h
 * Author: rbuck
 *
 * Created on October 21, 2010, 6:41 PM
 */

#ifndef _QUEUE_H
#define	_QUEUE_H


#include <queue>
#include <semaphore.h>
#include <typeinfo>

using namespace std;


// do we want to make this like a python Queue object?  Only a few more functions to go...
template<class T>
class Queue {
public:
    Queue() {
        sem_init(&sem_, 0, 1);
        sem_init(&counter_, 0, 0);
    }

    ~Queue() {

    }

    // do we want to do a timed wait? if so use: sem_timedwait() see the sem_wait man page
    T dequeue() {
        sem_wait(&counter_);
        sem_wait(&sem_);
        T value = q_.front();
        q_.pop();
        sem_post(&sem_);
        return value;
    }

    void enqueue(T obj) {
        sem_wait(&sem_);
        q_.push(obj);
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

    queue<T> q_;
};


#endif	/* _QUEUE_H */

