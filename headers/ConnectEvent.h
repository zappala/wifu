/* 
 * File:   ConnectEvent.h
 * Author: rbuck
 *
 * Created on October 29, 2010, 2:27 PM
 */

#ifndef _CONNECTEVENT_H
#define	_CONNECTEVENT_H

#include "Event.h"
#include "ISocketModule.h"


class ConnectEvent : public Event {

public:
    ConnectEvent(int socket) : Event(socket) {

    }

    void execute(IModule* m) {
        m->connect(this);
    }
};

#endif	/* _CONNECTEVENT_H */

