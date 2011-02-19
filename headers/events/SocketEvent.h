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
    SocketEvent(string& message, string& file, Socket* socket);

    virtual ~SocketEvent();

    void execute(IModule* m);

};

#endif	/* SOCKETEVENT_H */
