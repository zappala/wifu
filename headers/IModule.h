/* 
 * File:   IModule.h
 * Author: rbuck
 *
 * Created on October 20, 2010, 12:34 PM
 */

#ifndef _IMODULE_H
#define	_IMODULE_H

#include "Event.h"

class Event;

using namespace std;

class IModule {
public:
    IModule() {

    }

    virtual ~IModule(){

    }

    virtual void timeout(Event* e) {}
    virtual void timer_fired(Event* e) {}
    virtual void connect(Event* e) {}
    virtual void udp_send(Event* e) {}
    virtual void data(Event* e) {}

};

#endif	/* _IMODULE_H */

