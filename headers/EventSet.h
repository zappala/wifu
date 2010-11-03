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

class EventSet {
public:

    EventSet() {
        sem_.init(1);
    }

    virtual ~EventSet() {

    }

    virtual void add(Event* event) {
        sem_.wait();
        ids_.insert(event->get_id());
        sem_.post();
    }

    virtual void remove(Event* event) {
        sem_.wait();
        ids_.erase(event->get_id());
        sem_.post();
    }

    virtual bool contains(Event* event) {
        sem_.wait();
        bool val = ids_.find(event->get_id()) != ids_.end();
        sem_.post();
        return val;
    }

private:
    tr1::unordered_set<int> ids_;
    Semaphore sem_;
};

#endif	/* _EVENTSET_H */

