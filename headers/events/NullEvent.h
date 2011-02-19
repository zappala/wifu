/* 
 * File:   NullEvent.h
 * Author: rbuck
 *
 * Created on February 19, 2011, 9:49 AM
 */

#ifndef _NULLEVENT_H
#define	_NULLEVENT_H

#include "Event.h"

class NullEvent : public Event {
public:
    NullEvent(Socket*);

    void execute(IModule*);

};

#endif	/* _NULLEVENT_H */
