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
#include "TimeoutHelper.h"

class Module : public IModule, public QueueProcessor<Event*>, public TimeoutHelper {
public:
    Module() : IModule(), QueueProcessor<Event*>(&queue_), TimeoutHelper() {
        start_processing();
    }

    Module(IQueue<Event*>* queue) : IModule(), QueueProcessor<Event*>(queue), TimeoutHelper() {
        start_processing();
    }

    virtual ~Module() {
        
    }

    virtual void process(Event* e) {
        e->execute(this);
    }

    virtual void timer_fired(Event* e) {
        TimerFiredEvent* event = (TimerFiredEvent*) e;
        
        if(is_my_timeout(event)) {
            my_timer_fired(event);
        }
    }
    
private:
    Queue<Event*> queue_;
};


#endif	/* _MODULE_H */

