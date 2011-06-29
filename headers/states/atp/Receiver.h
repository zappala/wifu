/*
 * receiveData.h
 *
 *  Created on: Jun 17, 2011
 *      Author: philipbl
 */

#ifndef RECEIVEDATA_H_
#define RECEIVEDATA_H_

#include "TCPATPCongestionControl.h"

#include "contexts/ATPCongestionControlContext.h"


class Receiver: public TCPATPCongestionControl {
public:
	Receiver();
	virtual ~Receiver();

    virtual void state_enter(Context* c);
    virtual void state_exit(Context* c);

    virtual void state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);
    virtual void state_send_packet(Context*, QueueProcessor<Event*>* q, SendPacketEvent* e);


private:
    double calculateDelay(u_int32_t max, u_int32_t previous, double beta);

};

#endif /* RECEIVEDATA_H_ */
