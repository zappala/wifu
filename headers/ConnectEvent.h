/* 
 * File:   ConnectEvent.h
 * Author: rbuck
 *
 * Created on October 29, 2010, 2:27 PM
 */

#ifndef _CONNECTEVENT_H
#define	_CONNECTEVENT_H

#include "Event.h"
#include "IModule.h"


class ConnectEvent : public Event {
    void execute(IQModule* m) {
        m->connect(this);
    }
};

#endif	/* _CONNECTEVENT_H */

