/* 
 * File:   TCPTahoeReliabilityState.h
 * Author: rbuck
 *
 * Created on May 6, 2011, 2:25 PM
 */

#ifndef TCPTAHOERELIABILITYSTATE_H
#define	TCPTAHOERELIABILITYSTATE_H

#include "State.h"

#include "events/TimerFiredEvent.h"
#include "events/SendPacketEvent.h"
#include "events/NetworkReceivePacketEvent.h"
#include "events/CancelTimerEvent.h"
#include "events/ResendPacketEvent.h"
#include "events/CancelTimerEvent.h"
#include "events/ResponseEvent.h"

#include "contexts/TCPTahoeReliabilityContext.h"

#include "packet/TCPPacket.h"
#include "packet/TCPTimestampOption.h"

#include "Math.h"

class TCPTahoeReliabilityState : public State {
public:
    TCPTahoeReliabilityState();
    virtual ~TCPTahoeReliabilityState();

    virtual void state_send_packet(Context* c, SendPacketEvent* e);
    virtual void state_timer_fired(Context* c, TimerFiredEvent* e);
    virtual void state_receive_packet(Context* c, NetworkReceivePacketEvent* e);
    virtual void state_receive_buffer_not_empty(Context* c, ReceiveBufferNotEmptyEvent* e);
    virtual void state_receive(Context* c, ReceiveEvent* e);

    void create_and_dispatch_ack(Socket* s);

private:
    void start_timer(Context* c, Socket* s);
    void reset_timer(Context* c, Socket* s);
    void cancel_timer(Context* c, Socket* s);

    void resend_data(Context* c, Socket* s);
    void create_and_dispatch_received_data(Context* c, ReceiveEvent* e);

    
};

#endif	/* TCPTAHOERELIABILITYSTATE_H */

