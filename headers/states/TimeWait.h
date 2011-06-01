/* 
 * File:   TimeWait.h
 * Author: rbuck
 *
 * Created on April 18, 2011, 4:37 PM
 */

#ifndef TIMEWAIT_H
#define	TIMEWAIT_H

#include "State.h"
#include "Closed.h"

#include "contexts/Context.h"
#include "contexts/ConnectionManagerContext.h"

#include "events/framework_events/TimerFiredEvent.h"
#include "events/framework_events/TimeoutEvent.h"
#include "events/framework_events/NetworkReceivePacketEvent.h"
#include "events/framework_events/CancelTimerEvent.h"
#include "events/protocol_events/DeleteSocketEvent.h"

#include "defines.h"

class TimeWait : public State {
public:
    TimeWait();
    virtual ~TimeWait();
    virtual void state_enter(Context* c);
    virtual void state_exit(Context* c);

    virtual void state_timer_fired(Context* c, TimerFiredEvent* e);
    virtual void state_receive_packet(Context* c, NetworkReceivePacketEvent* e);

private:
    void start_timer(Context* c);
    void stop_timer(Context* c);
    void restart_timer(Context* c);

    
    TimeoutEvent* timeout_event_;
};

#endif	/* TIMEWAIT_H */