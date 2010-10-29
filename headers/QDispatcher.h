/* 
 * File:   QDispatcher.h
 * Author: rbuck
 *
 * Created on October 27, 2010, 4:59 PM
 */

#ifndef _QDISPATCHER_H
#define	_QDISPATCHER_H

#include "IQModule.h"
#include "Event.h"
#include "QueueProcessor.h"
#include "defines.h"

#include <tr1/unordered_map>



using namespace std;

class QDispatcher : public QueueProcessor<Event*> {
public:

    QDispatcher() : QueueProcessor<Event*>(&events_) {

    }

    ~QDispatcher() {
        tr1::unordered_map<event_name, vector<IQModule*>*>::iterator itr = map_.begin();
        for (; itr != map_.end(); ++itr) {
            vector<IQModule*>* v = itr->second;
            delete v;
        }
    }

    void map_event(event_name name, IQModule * m) {
        if (map_[name] == NULL) {
            map_[name] = new vector<IQModule *>;
        }
        map_[name]->push_back(m);

        // turn on module's thread
        m->start_processing();
    }

    void process(Event * e) {
        vector<IQModule*>* modules = map_[typeid(*e).name()];

        for(int i = 0; i < modules->size(); i++) {
            modules->at(i)->enqueue(e);
        }
    }

private:
    tr1::unordered_map<event_name, vector<IQModule*>*> map_;
    Queue<Event*> events_;
};

#endif	/* _QDISPATCHER_H */

