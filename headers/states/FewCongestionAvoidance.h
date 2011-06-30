/* 
 * File:   FewCongestionAvoidance.h
 * Author: leer
 *
 * Created on June 28, 2011, 2:37 PM
 */

#ifndef _FEWCONGESTIONAVOIDANCE_H
#define	_FEWCONGESTIONAVOIDANCE_H

#include "states/TCPTahoeBaseCongestionControl.h"
#include "contexts/TCPFewCongestionControlContext.h"
#include "contexts/TCPFewCongestionControlContext.h"
#include "states/FewSlowStart.h"
#include "Math.h"

class FewCongestionAvoidance : public TCPTahoeBaseCongestionControl {
public:
    FewCongestionAvoidance();
    virtual ~FewCongestionAvoidance();

    virtual void set_cwnd(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);
    virtual void resend(Context* c, QueueProcessor<Event*>* q, ResendPacketEvent* e);
};

#endif	/* _FEWCONGESTIONAVOIDANCE_H */

