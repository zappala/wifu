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
    ConnectionEstablishedEvent(AcceptEvent*, Socket*);

    virtual ~ConnectionEstablishedEvent();

    void execute(IModule*);

    Socket* get_new_socket();

    AcceptEvent* get_accept_event();

private:
    Socket* new_socket_;
    AcceptEvent* event_;

};

#endif	/* _CONNECTIONESTABLISHEDEVENT_H */
