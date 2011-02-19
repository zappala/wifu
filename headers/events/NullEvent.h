/* 
 * File:   NullEvent.h
 * Author: rbuck
 *
 * Created on February 19, 2011, 9:49 AM
 */

#ifndef _NULLEVENT_H
#define	_NULLEVENT_H

#include "events/Event.h"

class NullEvent : public Event {
public:
    NullEvent(Socket* s) : Event(s) {}
    void execute(IModule* m) {
        // do nothing
    }
};

#endif	/* _NULLEVENT_H */

