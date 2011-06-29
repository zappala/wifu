/*
 * Waiting.h
 *
 *  Created on: Jun 17, 2011
 *      Author: philipbl
 */

#ifndef WAITING_H_
#define WAITING_H_

#include "TCPATPCongestionControl.h"

#include "Sender.h"
#include "Receiver.h"

class Waiting: public TCPATPCongestionControl {
public:
	Waiting();
	virtual ~Waiting();

    virtual void state_enter(Context* c);
    virtual void state_exit(Context* c);

    virtual void state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e);

    virtual void state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);

private:

};

#endif /* WAITING_H_ */
