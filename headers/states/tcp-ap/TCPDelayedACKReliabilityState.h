/* 
 * File:   TCPDelayedACKReliabilityState.h
 * Author: rbuck
 *
 * Created on May 6, 2011, 2:25 PM
 */

#ifndef TCPDELAYEDACKRELIABILITYSTATE_H
#define	TCPDELAYEDACKRELIABILITYSTATE_H

#include "states/TCPTahoeReliabilityState.h"
#include "contexts/TCPDelayedACKReliabilityContext.h"

class TCPDelayedACKReliabilityState : public TCPTahoeReliabilityState {
public:
    TCPDelayedACKReliabilityState();
    virtual ~TCPDelayedACKReliabilityState();

    virtual void state_timer_fired(Context* c, QueueProcessor<Event*>* q, TimerFiredEvent* e);

    virtual void state_set_socket_option(Context* c, QueueProcessor<Event*>* q, SetSocketOptionEvent* e);

protected:
    void start_ack_timer(Context* c, Socket* s);
    //void reset_ack_timer(Context* c, Socket* s);
    void cancel_ack_timer(Context* c, Socket* s);

    void create_and_dispatch_ack(Context* c, QueueProcessor<Event*>* q, Event* e);

    void handle_data(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);

    u_int16_t get_delay_based_on_seq_num(Context* c, u_int32_t seqnum);
    
};

#endif	/* TCPDelayedACKReliabilityState_H */

