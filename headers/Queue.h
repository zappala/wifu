/* 
 * File:   Queue.h
 * Author: rbuck
 *
 * Created on October 21, 2010, 6:41 PM
 */

#ifndef _QUEUE_H
#define	_QUEUE_H


#include <queue>
#include <typeinfo>
#include <signal.h>

#include "Semaphore.h"
#include "IQueue.h"
#include "defines.h"

using namespace std;


// do we want to make this like a python Queue object?  Only a few more functions to go...
template<class T>
class Queue : public IQueue<T> {
public:
    Queue() : IQueue<T>() {
        sem_.init(1);
        counter_.init(0);
    }

    ~Queue() {

    }

    // do we want to do a timed wait? if so use: sem_timedwait() see the sem_wait man page
    T dequeue() {
        counter_.wait();
        sem_.wait();
        T value = q_.front();
        q_.pop();
        sem_.post();
        return value;
    }

    void enqueue(T obj, bool signal = false) {
        sem_.wait();
        q_.push(obj);
        if(signal) {
            raise(SIG_ENQUEUE_EVENT);
        }
        sem_.post();
        counter_.post();
    }

    int size() {
        sem_.wait();
        int value = q_.size();
        sem_.post();
        return value;
    }

    bool isEmpty() {
        // protection is done in size()
        return size() == 0;
    }
    
private:
    Semaphore sem_;
    Semaphore counter_;

    queue<T> q_;
};


#endif	/* _QUEUE_H */

