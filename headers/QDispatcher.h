/* 
 * File:   QDispatcher.h
 * Author: rbuck
 *
 * Created on October 27, 2010, 4:59 PM
 */

#ifndef _QDISPATCHER_H
#define	_QDISPATCHER_H

#include "IModule.h"
#include "Event.h"
#include "QueueProcessor.h"
#include "defines.h"

#include <tr1/unordered_map>



using namespace std;



class QDispatcher : public QueueProcessor<Event*> {
public:
    QDispatcher() : QueueProcessor<Event*>(&events_){

    }

    ~QDispatcher(){

    }

    void map_event(Event * e, IModule * m) {
        event_name name = typeid(*e).name();

        if(map_[name] == NULL) {
            map_[name] = new vector<IModule *>;
        }
        map_[name]->push_back(m);
    }

    void process(Event * obj) {

    }

private:
    tr1::unordered_map<event_name, vector<IModule*> *> map_;
    Queue<Event*> events_;
};

#endif	/* _QDISPATCHER_H */

