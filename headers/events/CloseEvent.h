/* 
 * File:   CloseEvent.h
 * Author: rbuck
 *
 * Created on April 18, 2011, 5:16 PM
 */

#ifndef CLOSEEVENT_H
#define	CLOSEEVENT_H

#include "LibraryEvent.h"
#include "Socket.h"

class CloseEvent : public LibraryEvent {
public:
    CloseEvent(string& message, string& file, Socket* s);
    virtual ~CloseEvent();
    void execute(IModule* m);
};

#endif	/* CLOSEEVENT_H */

