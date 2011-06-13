/* 
 * File:   CongestionAvoidance.h
 * Author: rbuck
 *
 * Created on May 6, 2011, 2:23 PM
 */

#ifndef CONGESTIONAVOIDANCE_H
#define	CONGESTIONAVOIDANCE_H

#include "states/TCPTahoeBaseCongestionControl.h"
#include "contexts/TCPTahoeCongestionControlContext.h"
#include "states/SlowStart.h"

class CongestionAvoidance : public TCPTahoeBaseCongestionControl {
public:
    CongestionAvoidance();
    virtual ~CongestionAvoidance();

    virtual void set_cwnd(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);
    virtual void resend(Context* c, QueueProcessor<Event*>* q, ResendPacketEvent* e);
};

#endif	/* CONGESTIONAVOIDANCE_H */

