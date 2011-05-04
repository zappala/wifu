/* 
 * File:   ReliabilityContext.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 10:36 AM
 */

#ifndef RELIABILITYCONTEXT_H
#define	RELIABILITYCONTEXT_H

#include "Context.h"
#include "states/SimpleTCPReliability.h"
#include "AddressPort.h"
#include "Socket.h"
#include "events/ConnectEvent.h"
#include "events/AcceptEvent.h"
#include "packet/WiFuPacket.h"
#include "packet/TCPPacket.h"
#include "events/TimerFiredEvent.h"
#include "events/TimeoutEvent.h"

class SimpleTCPReliabilityContext : public Context {
public:
    SimpleTCPReliabilityContext();
    virtual ~SimpleTCPReliabilityContext();

    void set_seq_number(u_int32_t value);
    u_int32_t get_seq_number();

    void set_ack_number(u_int32_t value);
    u_int32_t get_ack_number();

    void set_timeout_event(TimeoutEvent* e);
    TimeoutEvent* get_timeout_event();

private:
    u_int32_t seq_number_;
    u_int32_t ack_number_;

    TimeoutEvent* timeout_event_;
};

#endif	/* RELIABILITYCONTEXT_H */

