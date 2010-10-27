/* 
 * File:   Queue.h
 * Author: rbuck
 *
 * Created on October 21, 2010, 6:41 PM
 */

#ifndef _PRIORITYQUEUE_H
#define	_PRIORITYQUEUE_H


#include <queue>
#include <typeinfo>
#include <signal.h>

#include "defines.h"
#include "TimeoutEvent.h"
#include "Semaphore.h"
#include "IQueue.h"

using namespace std;


// do we want to make this like a python Queue object?  Only a few more functions to go...

template<class T, class Comparator>
class PriorityQueue : public IQueue<T> {
public:

    PriorityQueue() : IQueue<T>() {
        sem_.init(1);
        counter_.init(0);
        cout << "Priority Queue Created" << endl;
    }

    virtual ~PriorityQueue() {

    }

    // do we want to do a timed wait? if so use: sem_timedwait() see the sem_wait man page

    T dequeue() {
        counter_.wait();
        sem_.wait();
        T value = q_.top();
        q_.pop();
        sem_.post();
        return value;
    }

    void enqueue(T obj, bool signal = true) {
        sem_.wait();
        q_.push(obj);
        if (signal) {
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

    priority_queue<T, deque<T>, Comparator> q_;
};


#endif	/* _PRIORITYQUEUE_H */

