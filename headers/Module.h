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
class Module : public IModule, public QueueProcessor<Event*> {
public:

    /**
     * Constructs a Module object.
     * Uses a Queue as the internal data structure for the parent QueueProcessor object.
     * Starts up the parent QueueProcessor's listening thread.
     */
    Module();

    /**
     * Constructs a Module object.
     * Uses queue as the internal data structure for the parent QueueProcessor object.
     *
     * @param queue Pointer to an IQueue object to be used as the internal data structure for the parent QueueProcessor object.
     */
    Module(IQueue<Event*>* queue);

    /**
     * Cleans up this Module object
     */
    virtual ~Module();

    /**
     * Callback function called when an Event pointer is dequeued from the parent QueueProcessor's dequeue thread.
     *
     * @param e The Event to process.  This Event was dequeued from the parent QueueProcessor's dequeue thread.
     * @see Event::execute()
     */
    virtual void process(Event* e);

    /**
     * Simple wrapper method that enqueues e into the Dispatcher.
     *
     * @param e The Event pointer to enqueue into the Dispatcher.
     */
    virtual void dispatch(Event* e);

};


#endif	/* _MODULE_H */

