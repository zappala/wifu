/* 
 * File:   Event.h
 * Author: rbuck
 *
 * Created on October 21, 2010, 12:46 PM
 */

#ifndef _EVENT_H
#define	_EVENT_H

#include "Identifiable.h"
#include "IModule.h"

class IModule;

class Event : public Identifiable {
public:
    Event(int socket);
    virtual ~Event();
    virtual void execute(IModule* m) = 0;
    int & get_socket();
    virtual bool less_than(Event* rhs);
private:
    int socket_;
    
};

class EventComparator {
public:
    bool operator()(Event*& t1, Event*& t2) {
        return t1->less_than(t2);
    }
};



#endif	/* _EVENT_H */

