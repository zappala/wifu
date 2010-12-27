/* 
 * File:   EventSet.h
 * Author: rbuck
 *
 * Created on November 1, 2010, 5:31 PM
 */

#ifndef _EVENTSET_H
#define	_EVENTSET_H

#include <tr1/unordered_set>

#include "Event.h"
#include "Semaphore.h"

using namespace std;

/**
 * Keeps a set of Events.
 * This class is thread safe.
 */
class EventSet {
public:

    /**
     * Construct a new EventSet object.
     */
    EventSet() {
        sem_.init(1);
    }

    /**
     * Cleans up this EventSet object.
     */
    virtual ~EventSet() {

    }

    /**
     * Adds event to the set of internal Event objects.
     *
     * @param event The event to add to the set of internal Event objects.
     */
    virtual void add(Event* event) {
        sem_.wait();
        ids_.insert(event);
        sem_.post();
    }

    /**
     * Removes event from the set of internal Event objects.
     *
     * @param event The event to remove from the set of internal Event objects.
     */
    virtual void remove(Event* event) {
        sem_.wait();
        ids_.erase(event);
        sem_.post();
    }

    /**
     * Checks to see if event is in the set of internal Event objects.
     *
     * @param event The event to check.
     * @return True if event is in the set, false otherwise.
     */
    virtual bool contains(Event* event) {
        sem_.wait();
        bool val = ids_.find(event) != ids_.end();
        sem_.post();
        return val;
    }

    /**
     * Returns the number of elements in this EventSet
     *
     * @return the number of elements in this EventSet
     */
    virtual int size() {
        sem_.wait();
        int size = ids_.size();
        sem_.post();
        return size;
    }

private:
    /**
     * Set of ints which represent the ids of many Event objects.
     */
    tr1::unordered_set<Event*> ids_;

    /**
     * Semaphore which turns this class into a monitor.
     */
    Semaphore sem_;
};

#endif	/* _EVENTSET_H */

