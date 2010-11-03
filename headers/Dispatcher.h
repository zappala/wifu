/* 
 * File:   Dispatcher.h
 * Author: rbuck
 *
 * Created on October 27, 2010, 4:59 PM
 */

#ifndef _DISPATCHER_H
#define	_DISPATCHER_H

#include "Event.h"
#include "QueueProcessor.h"
#include "defines.h"

#include <tr1/unordered_map>

using namespace std;

class Dispatcher : public QueueProcessor<Event*> {
public:

    static Dispatcher & instance() {
        static Dispatcher instance_;
        return instance_;
    }

    ~Dispatcher() {
        tr1::unordered_map<event_name, vector<QueueProcessor<Event*>*>*>::iterator itr = map_.begin();
        for (; itr != map_.end(); ++itr) {
            vector<QueueProcessor<Event*>*>* v = itr->second;
            delete v;
        }
    }

    void map_event(event_name name, QueueProcessor<Event*>* q) {
        if (map_[name] == NULL) {
            map_[name] = new vector<QueueProcessor<Event*>*>;
        }
        map_[name]->push_back(q);
    }

    void process(Event * e) {
        vector<QueueProcessor<Event*>*>* queue_processors = map_[typeid(*e).name()];

        if(queue_processors == NULL) {
            return;
        }
        
        for(int i = 0; i < queue_processors->size(); i++) {
            queue_processors->at(i)->enqueue(e);
        }
    }

private:
    tr1::unordered_map<event_name, vector<QueueProcessor<Event*>*>*> map_;
    Queue<Event*> events_;


    Dispatcher() : QueueProcessor<Event*>(&events_) {

    }
};

#endif	/* _DISPATCHER_H */

