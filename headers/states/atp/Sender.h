/*
 * Sender.h
 *
 *  Created on: Jun 17, 2011
 *      Author: philipbl
 */

#ifndef SENDERACK_H_
#define SENDERACK_H_

#include "tcp-ap/SendRateLimiter.h"
#include "contexts/ATPCongestionControlContext.h"

#include "packet/ATPPacket.h"

class Sender: public SendRateLimiter {
public:
	Sender();
	virtual ~Sender();

    virtual void state_enter(Context* c);
    virtual void state_exit(Context* c);

    virtual void state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);
    virtual void state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e);

private:
    typedef SendRateLimiter super;

};

#endif /* SENDERACK_H_ */
