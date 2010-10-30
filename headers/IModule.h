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
    
    virtual void connect(Event* e) = 0;
    virtual void data(Event* e) = 0;

};

#endif	/* _IMODULE_H */

