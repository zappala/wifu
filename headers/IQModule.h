/* 
 * File:   IModule.h
 * Author: rbuck
 *
 * Created on October 20, 2010, 12:34 PM
 */

#ifndef _IQMODULE_H
#define	_IQMODULE_H

#include "QueueProcessor.h"
#include "Queue.h"
#include "Event.h"

class Event;

using namespace std;

class IQModule : public QueueProcessor<Event*> {
public:

    IQModule() : QueueProcessor<Event*>(&queue_) {

    }

    virtual ~IQModule() {

    }
    virtual void connect(Event* e) = 0;
    virtual void data(Event* e) = 0;

private:
    Queue<Event*> queue_;

};

#endif	/* _IQMODULE_H */

