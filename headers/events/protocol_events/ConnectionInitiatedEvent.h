/* 
 * File:   ConnectionInitiatedEvent.h
 * Author: rbuck
 *
 * Created on March 4, 2011, 2:17 PM
 */

#ifndef _CONNECTIONINITIATEDEVENT_H
#define	_CONNECTIONINITIATEDEVENT_H

#include "events/Event.h"

class ConnectionInitiatedEvent : public Event {
public:
    ConnectionInitiatedEvent(Socket* listening_socket, Socket* new_socket);

    virtual ~ConnectionInitiatedEvent();

    void execute(IModule* m);

    Socket* get_new_socket();


private:
    Socket* new_socket_;

};

#endif	/* _CONNECTIONINITIATEDEVENT_H */

