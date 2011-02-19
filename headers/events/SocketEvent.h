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
    SocketEvent(string&, string&, Socket*);

    virtual ~SocketEvent();

    void execute(IModule* );

};

#endif	/* SOCKETEVENT_H */
