/* 
 * File:   PassThroughRateLimiter.h
 * Author: leer
 *
 * Created on April 25, 2011, 11:39 AM
 */

#include "events/protocol_events/SendPacketEvent.h"
#include "events/framework_events/NetworkSendPacketEvent.h"
#include "State.h"

#ifndef _PASSTHROUGHRATELIMITER_H
#define	_PASSTHROUGHRATELIMITER_H

class PassThroughRateLimiter : public State {
public:
    /*Simple pass-through placeholder for rate limiters.*/
    void state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e);
};

#endif	/* _PASSTHROUGHRATELIMITER_H */

