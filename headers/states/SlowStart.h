/* 
 * File:   SlowStart.h
 * Author: rbuck
 *
 * Created on May 6, 2011, 2:22 PM
 */

#ifndef SLOWSTART_H
#define	SLOWSTART_H

#include "contexts/TCPTahoeCongestionControlContext.h"
#include "states/DummyCongestionController.h"
#include "states/CongestionAvoidance.h"

class SlowStart : public DummyCongestionController {
public:
    SlowStart();
    virtual ~SlowStart();

    virtual void state_enter(Context* c);
    virtual void set_cwnd(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);
    virtual void resend(Context* c, QueueProcessor<Event*>* q, ResendPacketEvent* e);

};

#endif	/* SLOWSTART_H */

