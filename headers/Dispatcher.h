/* 
 * File:   Dispatcher.h
 * Author: rbuck
 *
 * Created on October 27, 2010, 4:59 PM
 */

#ifndef _DISPATCHER_H
#define	_DISPATCHER_H

#include "events/Event.h"
#include "QueueProcessor.h"
#include "Queue.h"
#include "defines.h"

#include <algorithm>
#include <tr1/unordered_map>

using namespace std;

/**
 * Core of the application.  Receives all Event objects and enqueue's them to all registered QueueProcessor objects.
 * This class is a Singleton.
 */
class Dispatcher : public QueueProcessor<Event*> {
public:

    /**
     * @return A reference to the static instance of this Dispatcher object.
     */
    static Dispatcher & instance();

    /**
     * Cleans up this Dispatcher object.
     */
    ~Dispatcher();

    /**
     * Maps an event name to a QueueProcessor object.
     *
     * @param name The name of the Event.
     * @param q Pointer to a QueuProcessor object which wants to receive Event objects denoted by name.
     */
    void map_event(event_name, QueueProcessor<Event*>*);

    /**
     * Resets all mappings from event name to QueueProcessors and deletes the QueueProcessors.
     */
    void reset();

    /**
     * Callback function which is called upon a dequeue of an Event pointer on this object.
     * After dequeing the Event pointer, a copy of the pointer is enqueued into all registered QueueProcessor objects.
     * The enqueing to each QueueProcessor object is done in the order of registration.
     * I.e., if QueueProcessor A is registered before QueueProcessor B for Event objects of type C,
     * upon receiving an Event of type C, it will be enqueued to A first, then B.
     *
     * @see DequeueCallback<T>::process()
     */
    void process(Event*);

    /**
     * @see QueueProcessor::process()
     */
    bool should_enqueue(Event* event);

private:
    /**
     * Map of Event name to a vector of QueueProcessor objects.
     */
    tr1::unordered_map<event_name, vector<QueueProcessor<Event*>*>*> map_;

    /**
     * Mutex to make this object thread-safe
     */
    Semaphore mutex_;

    /**
     * Constructs a Dispatcher object.
     */
    Dispatcher();

    /**
     * Is called by reset() and the destructor.
     */
    void clear();
};

#endif	/* _DISPATCHER_H */

