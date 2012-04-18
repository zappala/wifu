/* 
 * File:   QueueProcessor.h
 * Author: rbuck
 *
 * Created on October 27, 2010, 11:35 AM
 */

#ifndef _QUEUEPROCESSOR_H
#define	_QUEUEPROCESSOR_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "IQueue.h"
#include "Queue.h"
#include "events/Event.h"
#include "DequeueCallback.h"
#include "Semaphore.h"

using namespace std;

/**
 * Struct to pass data from the constructor of QueueProcessor and its dequeuing thread.
 */
template<class T>
struct QueueProcessorStruct {
    IQueue<T> * queue;
    DequeueCallback<T> * callback;
    Semaphore * sem;
    Semaphore * start;
};

/**
 * Thread to dequeue objects from.  Calls DequeueCallback<T>::process() upon dequeing an element.
 */
template<class T>
void* thread(void * arg) {
    struct QueueProcessorStruct<T> * obj = (struct QueueProcessorStruct<T> *) arg;

    DequeueCallback<T> * callback = obj->callback;
    IQueue<T> * queue = obj->queue;
    Semaphore * start = obj->start;

    obj->sem->post();
    start->wait();

    while (true) {
        callback->process(queue->dequeue());
    }
}

/**
 * Class which provides an interface for external object to enqueue elements into an internal queue
 * and dequeue's those same elements in a thread.  In the thread, the process() method is called,
 * which makes a callback to this QueueProcessor object.
 */
template<class T>
class QueueProcessor : public DequeueCallback<T> {
public:

    /**
     * Constructs a QueueProcessor object.
     * Spawns a thread to dequeue elements from.
     *
     * @param queue A pointer to an IQueue which will be the underlying queue used.
     */
    QueueProcessor(IQueue<T>* queue = new Queue<Event*>()) : DequeueCallback<T>(), queue_(queue) {

        sem_.init(0);
        start_.init(0);

        struct QueueProcessorStruct<T> obj;
        obj.callback = this;
        obj.queue = queue_;
        obj.sem = &sem_;
        obj.start = &start_;

        if (pthread_create(&thread_, NULL, &thread<T>, &obj) != 0) {
            perror("Error creating new thread");
            exit(EXIT_FAILURE);
        }

        sem_.wait();
    }

    /**
     * Cleans up this QueueProcessor object.
     */
    virtual ~QueueProcessor() {
        if (pthread_cancel(thread_)) {
            cout << "Error canceling thread in Queue Processor: " << errno << endl;
        }
    }

    /**
     * Enqueue's obj into the internal IQueue.
     * If signal is true, raises a SIG_ENQUEUE_EVENT to the application.
     * By default, signal is false.
     *
     * @param object The element to enqueue.
     * @param signal If true, raises a SIG_ENQUEUE_EVENT signal.
     */
    virtual void enqueue(T object, bool signal = false) {
        if (should_enqueue(object)) {
            queue_->enqueue(object, signal);
        }
    }

    /**
     * Determine if object should be enqueued for processing
     * This function returns true always, overriding classes could change this.
     *
     * @param object The object to test whether it should be enqueued or not.
     * @return Whether this object should be enqueued for later processing.
     */
    virtual bool should_enqueue(T object) {
        return true;
    }

    /**
     * Lets the dequeue thread know it is okay to start dequeing elements.
     */
    void start_processing() {
        start_.post();
    }

    int size() {
        return queue_->size();
    }

private:
    /**
     * Pointer to an IQueue which points to either a Queue or PriorityQueue object.
     */
    IQueue<T>* queue_;

    /**
     * Thread which dequeue's elements from queue_.
     */
    pthread_t thread_;

    /**
     * Signaling Semaphore used to indicate to the QueueProcessor constructor
     * that the thread has started and has copied all necessary variables which were passed to it.
     */
    Semaphore sem_;

    /**
     * Semaphore used to indicate to thread_ that it is okay to start dequeing objects from queue_.
     */
    Semaphore start_;
};



#endif	/* _QUEUEPROCESSOR_H */

