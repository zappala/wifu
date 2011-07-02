/*
 * Sender.h
 *
 *  Created on: Jun 17, 2011
 *      Author: philipbl
 */

#ifndef SENDERACK_H_
#define SENDERACK_H_

#include "TCPATPCongestionControl.h"
#include "contexts/ATPCongestionControlContext.h"

class Sender: public TCPATPCongestionControl {
public:
	Sender();
	virtual ~Sender();

    virtual void state_enter(Context* c);
    virtual void state_exit(Context* c);

    virtual void state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);
    virtual void state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e);
    virtual void state_send_buffer_not_empty(Context* c, QueueProcessor<Event*>* q, SendBufferNotEmptyEvent* e);

private:
    typedef TCPATPCongestionControl super;

    void update_sending_rate(Context * c, ATPPacket * p);



};

#endif /* SENDERACK_H_ */
