/* 
 * File:   ReliabilityContext.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 10:36 AM
 */

#ifndef RELIABILITYCONTEXT_H
#define	RELIABILITYCONTEXT_H

#include "Context.h"
#include "states/ReliabilityState.h"
#include "AddressPort.h"
#include "Socket.h"
#include "events/ConnectEvent.h"
#include "events/AcceptEvent.h"
#include "packet/WiFuPacket.h"
#include "packet/TCPPacket.h"
#include "events/TimerFiredEvent.h"
#include "events/TimeoutEvent.h"

class ReliabilityContext : public Context {
public:
    ReliabilityContext();
    virtual ~ReliabilityContext();

    void set_seq_number(u_int32_t value);
    u_int32_t get_seq_number();

    void set_ack_number(u_int32_t value);
    u_int32_t get_ack_number();

private:
    u_int32_t seq_number_;
    u_int32_t ack_number_;
};

#endif	/* RELIABILITYCONTEXT_H */

