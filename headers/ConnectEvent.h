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
    ConnectEvent(int socket, string & address) : Event(socket), address_(address) {

    }

    void execute(IModule* m) {
        cout << "Connect Execute" << endl;
        m->connect(this);
    }

    string & get_address() {
        return address_;
    }

private:
    string address_;
};

#endif	/* _CONNECTEVENT_H */

