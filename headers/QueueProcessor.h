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

#include "IQueue.h"
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
void * thread(void * arg) {
    struct QueueProcessorStruct<T> * obj = (struct QueueProcessorStruct<T> *) arg;

    DequeueCallback<T> * callback = obj->callback;
    IQueue<T> * queue = obj->queue;
    Semaphore * start = obj->start;

    obj->sem->post();
    start->wait();

    while (1) {
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
    QueueProcessor(IQueue<T>* queue) : DequeueCallback<T>(), queue_(queue) {

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

    }

    /**
     * Enqueue's obj into the internal IQueue.
     * If signal is true, raises a SIG_ENQUEUE_EVENT to the application.
     * By default, signal is false.
     *
     * @param obj The element to enqueue.
     * @param signal If true, raises a SIG_ENQUEUE_EVENT signal.
     */
    virtual void enqueue(T object, bool signal = false) {
        queue_->enqueue(object, signal);
    }

    /**
     * Lets the dequeue thread know it is okay to start dequeing elements.
     */
    void start_processing() {
        start_.post();
    }

private:
    IQueue<T>* queue_;
    pthread_t thread_;
    Semaphore sem_;
    Semaphore start_;
};



#endif	/* _QUEUEPROCESSOR_H */

