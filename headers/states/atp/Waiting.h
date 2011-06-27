/*
 * Waiting.h
 *
 *  Created on: Jun 17, 2011
 *      Author: philipbl
 */

#ifndef WAITING_H_
#define WAITING_H_

#include "contexts/ATPCongestionControlContext.h"
#include "State.h"
#include "Sender.h"
#include "Receiver.h"

#include "packet/ATPPacket.h"

class Waiting: public State {
public:
	Waiting();
	virtual ~Waiting();

    virtual void state_enter(Context* c);
    virtual void state_exit(Context* c);

    virtual void state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);

};

#endif /* WAITING_H_ */
