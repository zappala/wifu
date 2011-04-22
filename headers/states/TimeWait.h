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
#include "events/DeleteSocketEvent.h"
#include "states/Closed.h"
#include "defines.h"

class TimeWait : public State {
public:
    TimeWait();
    virtual ~TimeWait();
    virtual void state_enter(Context* c);
    virtual void state_exit(Context* c);

    virtual void state_timer_fired(Context* c, TimerFiredEvent* e);

private:
    TimeoutEvent* timeout_event_;
};

#endif	/* TIMEWAIT_H */