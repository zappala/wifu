/* 
 * File:   FrameworkEvent.h
 * Author: rbuck
 *
 * Created on June 1, 2011, 1:44 PM
 */

#ifndef FRAMEWORKEVENT_H
#define	FRAMEWORKEVENT_H

#include "events/Event.h"
#include "defines.h"

class FrameworkEvent : public Event {
public:
    FrameworkEvent(Socket* s);
    virtual ~FrameworkEvent();
};

#endif	/* FRAMEWORKEVENT_H */

