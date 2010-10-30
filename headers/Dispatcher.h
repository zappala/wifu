/* 
 * File:   Dispatcher.h
 * Author: rbuck
 *
 * Created on October 27, 2010, 4:59 PM
 */

#ifndef _DISPATCHER_H
#define	_DISPATCHER_H

#include "Module.h"
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
        tr1::unordered_map<event_name, vector<Module*>*>::iterator itr = map_.begin();
        for (; itr != map_.end(); ++itr) {
            vector<Module*>* v = itr->second;
            delete v;
        }
    }

    void map_event(event_name name, Module * m) {
        if (map_[name] == NULL) {
            map_[name] = new vector<Module *>;
        }
        map_[name]->push_back(m);

        // turn on module's thread
        m->start_processing();
    }

    void process(Event * e) {
        vector<Module*>* modules = map_[typeid(*e).name()];

        for(int i = 0; i < modules->size(); i++) {
            modules->at(i)->enqueue(e);
        }
    }

private:
    tr1::unordered_map<event_name, vector<Module*>*> map_;
    Queue<Event*> events_;


    Dispatcher() : QueueProcessor<Event*>(&events_) {

    }
};

#endif	/* _DISPATCHER_H */

