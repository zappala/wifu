/* 
 * File:   TimeWait.h
 * Author: rbuck
 *
 * Created on April 18, 2011, 4:37 PM
 */

#ifndef TIMEWAIT_H
#define	TIMEWAIT_H

#include "State.h"
#include "contexts/Context.h"
#include "events/TimerFiredEvent.h"
#include "events/TimeoutEvent.h"
#include "contexts/ConnectionManagerContext.h"

class TimeWait : public State {
public:
    TimeWait();
    virtual ~TimeWait();
    virtual void enter(Context* c);
    virtual void exit(Context* c);

    virtual void timer_fired(Context* c, TimerFiredEvent* e);
};

#endif	/* TIMEWAIT_H */