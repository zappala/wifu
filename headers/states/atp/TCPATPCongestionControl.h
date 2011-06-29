/*
 * TCPATPCongestionControl.h
 *
 *  Created on: Jun 23, 2011
 *      Author: philipbl
 */

#ifndef TCPATPCONGESTIONCONTROL_H_
#define TCPATPCONGESTIONCONTROL_H_

#include "events/Event.h"
#include "events/framework_events/TimeoutEvent.h"
#include "events/framework_events/NetworkSendPacketEvent.h"
#include "events/protocol_events/SendPacketEvent.h"
#include "events/framework_events/TimerEvent.h"
#include "events/framework_events/TimerFiredEvent.h"

#include "packet/WiFuPacket.h"
#include "packet/ATPPacket.h"
#include "packet/TCPTimestampOption.h"
#include "contexts/Context.h"
#include "Queue.h"

#include "State.h"

class TCPATPCongestionControl: public State {
public:
	TCPATPCongestionControl();
	virtual ~TCPATPCongestionControl();

	void setRate(int seconds,  long int nanoseconds);

	int getSeconds();

	long int getNanoseconds();

	virtual void state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e);
    virtual void state_resend_packet(Context* c, QueueProcessor<Event*>* q, ResendPacketEvent* e);
    virtual void state_timer_fired(Context* c, QueueProcessor<Event*>* q, TimerFiredEvent* e);

protected:
	void send_packet(ATPPacket* p, Socket* s);
	void send_packets(Context* c, QueueProcessor<Event*>* q, Event* e);
	void delay_send_packets(Context* c, QueueProcessor<Event*>* q, Event* e);
	void printPacket(ATPPacket* packet);

    void hexDump (char *desc, void *addr, int len);




private:
    bool canSend_;
    int sendSeconds_;
    long int sendNanos_;
    TimeoutEvent* waitingTimeout_;
    Queue<WiFuPacket*>* queue_;

    void setTimer(Socket* s);
	int get_resend_data_length(Context* c, Event* e, WiFuPacket* p);
	void resend_data(Context* c, QueueProcessor<Event*>* q, Event* e);



};

#endif /* TCPATPCONGESTIONCONTROL_H_ */
