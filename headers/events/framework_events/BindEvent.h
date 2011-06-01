/* 
 * File:   BindEvent.h
 * Author: rbuck
 *
 * Created on December 27, 2010, 2:27 PM
 */

#ifndef BINDEVENT_H
#define	BINDEVENT_H

#include "LibraryEvent.h"

using namespace std;

class BindEvent : public LibraryEvent {
public:
    BindEvent(string& message, string& file, Socket* s);

    virtual ~BindEvent();

    void execute(IModule* m);
    
};

#endif	/* BINDEVENT_H */
