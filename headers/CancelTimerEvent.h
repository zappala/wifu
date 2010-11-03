/* 
 * File:   CancelTimerEvent.h
 * Author: rbuck
 *
 * Created on November 3, 2010, 2:56 PM
 */

#ifndef _CANCELTIMEREVENT_H
#define	_CANCELTIMEREVENT_H

#include "TimerEvent.h"

class CancelTimerEvent : public TimerEvent {
public:
    CancelTimerEvent(TimeoutEvent* event) : TimerEvent(event) {

    }
    
    virtual ~CancelTimerEvent() {
        
    }

    void execute(IModule* m) {
        m->cancel_timer(this);
    }
};

#endif	/* _CANCELTIMEREVENT_H */

