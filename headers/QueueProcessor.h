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

#include "Queue.h"
#include "DequeueCallback.h"
#include "Semaphore.h"

using namespace std;

template<class T>
struct QueueProcessorStruct {
    IQueue<T> * queue;
    DequeueCallback<T> * callback;
    Semaphore * sem;
    Semaphore * start;
};

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

template<class T>
class QueueProcessor {
public:

    QueueProcessor(IQueue<T>* queue, DequeueCallback<T>* callback)
        : queue_(queue), callback_(callback) {

        sem_.init(0);
        start_.init(0);


        struct QueueProcessorStruct<T> obj;
        obj.callback = callback_;
        obj.queue = queue_;
        obj.sem = &sem_;
        obj.start = &start_;


        if (pthread_create(&thread_, NULL, &thread<T>, &obj) != 0) {
            perror("Error creating new thread");
            exit(EXIT_FAILURE);
        }

        sem_.wait();
    }

    virtual ~QueueProcessor() {

    }

    void enqueue(T object, bool signal = true) {
        queue_->enqueue(object, signal);
    }

    void start() {
        start_.post();
    }

private:
    IQueue<T>* queue_;
    DequeueCallback<T>* callback_;
    pthread_t thread_;
    Semaphore sem_;
    Semaphore start_;
};



#endif	/* _QUEUEPROCESSOR_H */

