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
class QueueProcessor : public DequeueCallback<T> {
public:

    QueueProcessor(IQueue<T>* queue = new Queue<T>()) : DequeueCallback<T>(), queue_(queue) {

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

    virtual ~QueueProcessor() {
        cout << "Q ptr " << typeid(*queue_).name() << endl;
        cout << "Q " << typeid(Queue<T>).name() << endl;
        if(typeid(*queue_) == typeid(Queue<T>)) {
            delete queue_;
        }
    }

    void enqueue_and_signal(T object) {
        enqueue(object, true);
    }

    void enqueue(T object, bool signal = false) {
        if (signal) {
            queue_->enqueue_and_signal(object);
        } else {
            queue_->enqueue(object);
        }

    }

    void start_processing() {
        start_.post();
    }

private:
    IQueue<T>* queue_;
    pthread_t thread_;
    Semaphore sem_;
    Semaphore start_;
    bool delete_queue_;
};



#endif	/* _QUEUEPROCESSOR_H */

