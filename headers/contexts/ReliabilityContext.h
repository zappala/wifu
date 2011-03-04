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

    void set_last_packet_sent(TCPPacket* p);
    TCPPacket* get_last_packet_sent();

    void set_last_packet_received(TCPPacket* p);
    TCPPacket* get_last_packet_received();

    void set_saved_timeout(TimeoutEvent* te);
    TimeoutEvent* get_saved_timeout();

    void set_seq_counter(u_int32_t value);
    u_int32_t get_seq_counter();

private:
    TCPPacket* last_packet_sent_;
    TCPPacket* last_packet_received_;
    u_int32_t seq_counter_;
    TimeoutEvent* saved_timeout_;
};

#endif	/* RELIABILITYCONTEXT_H */

