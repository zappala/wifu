/* 
 * File:   SocketEvent.h
 * Author: rbuck
 *
 * Created on December 27, 2010, 1:55 PM
 */

#ifndef SOCKETEVENT_H
#define	SOCKETEVENT_H

#include "LibraryEvent.h"
#include "Socket.h"

class SocketEvent : public LibraryEvent {
public:

    SocketEvent(string& message, string& file, Socket* socket) : LibraryEvent(message, file, socket) {

    }

    virtual ~SocketEvent() {

    }

    void execute(IModule* m) {
        m->library_socket(this);
    }
};

#endif	/* SOCKETEVENT_H */

