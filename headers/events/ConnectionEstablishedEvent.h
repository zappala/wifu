/* 
 * File:   ConnectionEstablishedEvent.h
 * Author: rbuck
 *
 * Created on January 14, 2011, 3:50 PM
 */

#ifndef _CONNECTIONESTABLISHEDEVENT_H
#define	_CONNECTIONESTABLISHEDEVENT_H

#include "Event.h"
#include "AcceptEvent.h"


class ConnectionEstablishedEvent : public Event {
public:
    ConnectionEstablishedEvent(AcceptEvent* e, Socket* new_socket) :
                            Event(e->get_socket()),
                            new_socket_(new_socket),
                            event_(e) {

    }

    virtual ~ConnectionEstablishedEvent() {

    }

    void execute(IModule* m) {
        m->connection_established(this);
    }

    Socket* get_new_socket() {
        return new_socket_;
    }

    AcceptEvent* get_accept_event() {
        return event_;
    }

private:
    Socket* new_socket_;
    AcceptEvent* event_;
};

#endif	/* _CONNECTIONESTABLISHEDEVENT_H */

