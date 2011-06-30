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

protected:
    void start_ack_timer(Context* c, Socket* s);
    //void reset_ack_timer(Context* c, Socket* s);
    void cancel_ack_timer(Context* c, Socket* s);

    void handle_data(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);

private:
    u_int16_t cur_ack_count_;
    u_int32_t ack_timeout_delay;
    
};

#endif	/* TCPDelayedACKReliabilityState_H */

