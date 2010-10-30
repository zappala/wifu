/* 
 * File:   IModule.h
 * Author: rbuck
 *
 * Created on October 20, 2010, 12:34 PM
 */

#ifndef _IMODULE_H
#define	_IMODULE_H

#include "QueueProcessor.h"
#include "Queue.h"
#include "Event.h"

class Event;

using namespace std;

class IModule {
public:
    IModule() {

    }

    virtual ~IModule(){

    }
    
    virtual void connect(Event* e) {}
    virtual void udp_send(Event* e) {}
    virtual void data(Event* e) {};

};

#endif	/* _IMODULE_H */

