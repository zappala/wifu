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
#include "Semaphore.h"
#include "IQueue.h"

using namespace std;

/**
 * Queue which gives priority to its elements based on Comparator.
 * This object is thread safe.
 * @see IQueue
 * @see Queue
 */
template<class T, class Comparator>
class PriorityQueue : public IQueue<T> {
public:

    /**
     * Constructs a PriorityQueue object.
     */
    PriorityQueue() : IQueue<T>() {
        sem_.init(1);
        counter_.init(0);
    }

    /**
     * Cleans up this PriorityQueue object.
     */
    virtual ~PriorityQueue() {

    }

    /**
     * Dequeues and returns the next element in the queue.
     * This is a blocking method and will wait indefinately if there is nothing to dequeue.
     *
     * @return The next element in the queue.
     */
    T dequeue() {
        counter_.wait();
        sem_.wait();
        T value = q_.top();
        q_.pop();
        sem_.post();
        return value;
    }

    /**
     * Enqueues obj into this object.  If signal is true, raises a SIG_ENQUEUE_EVENT signal.
     *
     * @param obj The element to put into this PriorityQueue.
     * @param signal If true, raises a SIG_ENQUEUE_EVENT signal.
     */
    void enqueue(T obj, bool signal = false) {
        sem_.wait();
        q_.push(obj);
        if (signal) {
            raise(SIG_ENQUEUE_EVENT);
        }
        sem_.post();
        counter_.post();
    }

    /**
     * @return The number of elements in this PriorityQueue.
     */
    int size() {
        sem_.wait();
        int value = q_.size();
        sem_.post();
        return value;
    }

    /**
     * @return True if there are no elements in this PriorityQueue, false otherwise.
     */
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

