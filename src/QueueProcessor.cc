#include "QueueProcessor.h"

QueueProcessor::QueueProcessor(DequeueCallback* callback) : callback_(callback) {

    sem_.init(0);

    struct QueueProcessorStruct obj;
    obj.callback = callback_;
    obj.queue = &queue_;
    obj.sem = &sem_;

    if (pthread_create(&thread_, NULL, &thread, &obj) != 0) {
        perror("Error creating new thread");
        exit(EXIT_FAILURE);
    }

    sem_.wait();
}

QueueProcessor::~QueueProcessor() {

}

void QueueProcessor::enqueue(Event* event) {
    queue_.enqueue(event);
}

void * thread(void * arg) {
    struct QueueProcessorStruct * obj = (struct QueueProcessorStruct *) arg;

    DequeueCallback * callback = obj->callback;
    Queue<Event*> * queue = obj->queue;

    obj->sem->post();

    while(1) {
        callback->process(queue->dequeue());
    }    
}