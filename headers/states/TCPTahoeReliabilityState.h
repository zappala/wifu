/* 
 * File:   TCPTahoeReliabilityState.h
 * Author: rbuck
 *
 * Created on May 6, 2011, 2:25 PM
 */

#ifndef TCPTAHOERELIABILITYSTATE_H
#define	TCPTAHOERELIABILITYSTATE_H

#include "State.h"

#include "events/framework_events/TimerFiredEvent.h"
#include "events/protocol_events/SendPacketEvent.h"
#include "events/framework_events/NetworkReceivePacketEvent.h"
#include "events/framework_events/CancelTimerEvent.h"
#include "events/protocol_events/ResendPacketEvent.h"
#include "events/framework_events/ResponseEvent.h"

#include "contexts/TCPTahoeReliabilityContext.h"

#include "packet/TCPPacket.h"
#include "packet/TCPTimestampOption.h"

#include "Math.h"

class TCPTahoeReliabilityState : public State {
public:
    TCPTahoeReliabilityState();
    virtual ~TCPTahoeReliabilityState();

    virtual void state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e);
    virtual void state_timer_fired(Context* c, QueueProcessor<Event*>* q, TimerFiredEvent* e);
    virtual void state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);
    virtual void state_receive_buffer_not_empty(Context* c, QueueProcessor<Event*>* q, ReceiveBufferNotEmptyEvent* e);
    virtual void state_receive(Context* c, QueueProcessor<Event*>* q, ReceiveEvent* e);

    void create_and_dispatch_ack(QueueProcessor<Event*>* q, Socket* s);

private:
    void start_timer(Context* c, Socket* s);
    void reset_timer(Context* c, Socket* s);
    void cancel_timer(Context* c, Socket* s);

    void update_rto(Context* c, TCPTimestampOption* ts);

    void resend_data(Context* c, QueueProcessor<Event*>* q, Socket* s);
    void create_and_dispatch_received_data(Context* c, QueueProcessor<Event*>* q, ReceiveEvent* e);

    
};

#endif	/* TCPTAHOERELIABILITYSTATE_H */

