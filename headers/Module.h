/* 
 * File:   Module.h
 * Author: rbuck
 *
 * Created on October 29, 2010, 5:38 PM
 */

#ifndef _MODULE_H
#define	_MODULE_H


#include "QueueProcessor.h"
#include "IModule.h"
#include "Queue.h"

class Module : public IModule, public QueueProcessor<Event*> {
public:
    Module() : IModule(), QueueProcessor<Event*>(&queue_) {
        cout << "Queue Size: " << queue_.size() << endl;

        start_processing();
    }

    Module(IQueue<Event*>* queue) : IModule(), QueueProcessor<Event*>(queue) {
        cout << "Queue Size: " << queue_.size() << endl;
        start_processing();
    }

    virtual ~Module() {
        
    }

    virtual void process(Event* e) {
        e->execute(this);
    }

private:
    Queue<Event*> queue_;
};


#endif	/* _MODULE_H */

