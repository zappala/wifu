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

/**
 * Represents the base class of many different aspects of a transport layer protocol.
 *
 * @see IModule
 * @see QueueProcessor
 * @see TimeoutHelper
 */
class Module : public IModule, public QueueProcessor<Event*>, public TimeoutHelper {
public:

    /**
     * Constructs a Module object.
     * Uses a Queue as the internal data structure for the parent QueueProcessor object.
     * Starts up the parent QueueProcessor's listening thread.
     */
    Module() : IModule(), QueueProcessor<Event*>(), TimeoutHelper() {
        start_processing();
    }

    /**
     * Constructs a Module object.
     * Uses queue as the internal data structure for the parent QueueProcessor object.
     *
     * @param queue Pointer to an IQueue object to be used as the internal data structure for the parent QueueProcessor object.
     */
    Module(IQueue<Event*>* queue) : IModule(), QueueProcessor<Event*>(queue), TimeoutHelper() {
        start_processing();
    }

    /**
     * Cleans up this Module object
     */
    virtual ~Module() {
        
    }

    /**
     * Callback function called when an Event pointer is dequeued from the parent QueueProcessor's dequeue thread.
     *
     * @param e The Event to process.  This Event was dequeued from the parent QueueProcessor's dequeue thread.
     * @see Event::execute()
     */
    virtual void process(Event* e) {
        e->execute(this);
    }

    /**
     * Called when e's execute method is called on a TimerFiredEvent.
     * Screens e to ensure that it belongs to this Module, then calls my_timer_fired().
     *
     * @param e The Event which caused this method to be called (likely a TimerFiredEvent).
     */
    virtual void timer_fired(Event* e) {
        TimerFiredEvent* event = (TimerFiredEvent*) e;
        
        if(is_my_timeout(event)) {
            my_timer_fired(event);
        }
    }

    /**
     * Simple wrapper method that enqueues e into the Dispatcher.
     *
     * @param e The Event pointer to enqueue into the Dispatcher.
     */
    virtual void dispatch(Event* e) {
        Dispatcher::instance().enqueue(e);
    }

};


#endif	/* _MODULE_H */

