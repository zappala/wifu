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

#include "contexts/TCPTahoeReliabilityContext.h"

class TCPTahoeReliabilityState : public State {
public:
    TCPTahoeReliabilityState();
    virtual ~TCPTahoeReliabilityState();

    virtual void state_send_packet(Context* c, SendPacketEvent* e);
    virtual void state_receive_packet(Context* c, NetworkReceivePacketEvent* e);
    virtual void state_timer_fired(Context* c, TimerFiredEvent* e);
};

#endif	/* TCPTAHOERELIABILITYSTATE_H */

