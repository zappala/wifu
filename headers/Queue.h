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

/**
 * FIFO queue.
 * This object is thread safe.
 * @see IQueue
 * @see PriorityQueue
 */
template<class T>
class Queue : public IQueue<T> {
public:

    /**
     * Constructs a Queue object.
     */
    Queue() : IQueue<T>() {
        sem_.init(1);
        counter_.init(0);
    }

    /**
     * Cleans up this Queue object.
     */
    virtual ~Queue() {

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
        T value = q_.front();
        q_.pop();
        sem_.post();
        return value;
    }

    /**
     * Enqueues obj into this object.  If signal is true, raises a SIG_ENQUEUE_EVENT signal.
     *
     * @param obj The element to put into this Queue.
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
     * @return The number of elements in this Queue.
     */
    int size() {
        sem_.wait();
        int value = q_.size();
        sem_.post();
        return value;
    }

    /**
     * @return True if there are no elements in this Queue, false otherwise.
     */
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

