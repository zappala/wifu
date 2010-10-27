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

struct QueueProcessorStruct {
    Queue<Event*> * queue;
    DequeueCallback * callback;
    Semaphore * sem;
};

//prototype
void * thread(void * arg);

class QueueProcessor {
public:

    QueueProcessor(DequeueCallback * callback);
    virtual ~QueueProcessor();
    void enqueue(Event * event);

private:
    Queue<Event *> queue_;
    DequeueCallback* callback_;
    pthread_t thread_;
    Semaphore sem_;
};

#endif	/* _QUEUEPROCESSOR_H */

