/* 
 * File:   ReceiveEvent.h
 * Author: rbuck
 *
 * Created on March 4, 2011, 5:38 PM
 */

#ifndef _RECEIVEEVENT_H
#define	_RECEIVEEVENT_H

#include "LibraryEvent.h"


class ReceiveEvent : public LibraryEvent {
public:
    ReceiveEvent(string& message, string& file, Socket* s);
    void execute(IModule* m);
};

#endif	/* _RECEIVEEVENT_H */

