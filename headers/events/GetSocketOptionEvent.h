/* 
 * File:   GetSocketOptionEvent.h
 * Author: rbuck
 *
 * Created on April 26, 2011, 2:13 PM
 */

#ifndef GETSOCKETOPTIONEVENT_H
#define	GETSOCKETOPTIONEVENT_H

#include "SocketOptionEvent.h"
#include "Socket.h"

class GetSocketOptionEvent : public SocketOptionEvent {
public:
    GetSocketOptionEvent(string& message, string& file, Socket* s);
    virtual ~GetSocketOptionEvent();

    void execute(IModule* m);
};


#endif	/* GETSOCKETOPTIONEVENT_H */