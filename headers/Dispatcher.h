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
    static Dispatcher & instance() {
        static Dispatcher instance_;
        return instance_;
    }

    /**
     * Cleans up this Dispatcher object.
     */
    ~Dispatcher() {
        tr1::unordered_map<event_name, vector<QueueProcessor<Event*>*>*>::iterator itr = map_.begin();
        for (; itr != map_.end(); ++itr) {
            vector<QueueProcessor<Event*>*>* v = itr->second;
            delete v;
        }
    }

    /**
     * Maps an event name to a QueueProcessor object.
     *
     * @param name The name of the Event.
     * @param q Pointer to a QueuProcessor object which wants to receive Event objects denoted by name.
     */
    void map_event(event_name name, QueueProcessor<Event*>* q) {
        if (map_[name] == NULL) {
            map_[name] = new vector<QueueProcessor<Event*>*>;
        }

        vector<QueueProcessor<Event*>*>::iterator itr = find(map_[name]->begin(), map_[name]->end(), q);
        if(itr != map_[name]->end()) {
            return;
        }

        map_[name]->push_back(q);
    }

    /**
     * Callback function which is called upon a dequeue of an Event pointer on this object.
     * After dequeing the Event pointer, a copy of the pointer is enqueued into all registered QueueProcessor objects.
     * The enqueing to each QueueProcessor object is done in the order of registration.
     * I.e., if QueueProcessor A is registered before QueueProcessor B for Event objects of type C,
     * upon receiving an Event of type C, it will be enqueued to A first, then B.
     *
     * @see DequeueCallback<T>::process()
     */
    void process(Event * e) {
        cout << "Event name: " << type_name(*e) << endl;
        vector<QueueProcessor<Event*>*>* queue_processors = map_[type_name(*e)];

        if (queue_processors == NULL) {
            return;
        }

        for (int i = 0; i < queue_processors->size(); i++) {
            cout << "Processing: " << type_name(*e) << endl;
            queue_processors->at(i)->enqueue(e);
        }
    }

private:
    /**
     * Map of Event name to a vector of QueueProcessor objects.
     */
    tr1::unordered_map<event_name, vector<QueueProcessor<Event*>*>*> map_;
    
    /**
     * IQueue to be used by the parent QueueProcessor object.
     */
    Queue<Event*> events_;

    /**
     * Constructs a Dispatcher object.
     */
    Dispatcher() : QueueProcessor<Event*>(&events_) {

    }
};

#endif	/* _DISPATCHER_H */

