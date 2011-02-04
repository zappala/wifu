/* 
 * File:   IQueue.h
 * Author: rbuck
 *
 * Created on October 27, 2010, 1:01 PM
 */

#ifndef _IQUEUE_H
#define	_IQUEUE_H

#include "GarbageCollector.h"

/**
 * Interface for a queue.
 */
template<class T>
class IQueue : public gc {
public:
    /**
     * Constructs an IQueue object.
     */
    IQueue() {}

    /**
     * Cleans up this IQueue object.
     */
    virtual ~IQueue() {}

    /**
     * Dequeues and returns the get element on the queue.
     *
     * @return The get element on the queue.
     */
    virtual T dequeue() = 0;

    /**
     * Enqueue's obj into this IQueue.
     * If signal is true, raises a SIG_ENQUEUE_EVENT to the application.
     * By default, signal is false.
     *
     * @param obj The element to enqueue.
     * @param signal If true, raises a SIG_ENQUEUE_EVENT signal.
     */
    virtual void enqueue(T obj, bool signal = false) = 0;

    /**
     * @return The number of elements in this IQueue.
     */
    virtual int size() = 0;

    /**
     * @return True if there are no elements in the queue, false otherwise.
     */
    virtual bool isEmpty() = 0;
};

#endif	/* _IQUEUE_H */

