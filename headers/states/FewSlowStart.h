/* 
 * File:   FewSlowStart.h
 * Author: leer
 *
 * Created on June 28, 2011, 2:36 PM
 */

#ifndef _FEWSLOWSTART_H
#define	_FEWSLOWSTART_H

#include "contexts/TCPFewCongestionControlContext.h"
#include "states/TCPTahoeBaseCongestionControl.h"
#include "states/FewCongestionAvoidance.h"

class FewSlowStart : public TCPTahoeBaseCongestionControl {
public:
    FewSlowStart();
    virtual ~FewSlowStart();

    virtual void state_enter(Context* c);
    virtual void set_cwnd(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);
    virtual void resend(Context* c, QueueProcessor<Event*>* q, ResendPacketEvent* e);

};

#endif	/* _FEWSLOWSTART_H */

