/* 
 * File:   TCPDelayedACKReliabilityContext.h
 * Author: leer
 *
 * Created on June 17, 2011, 2:09 PM
 */

#ifndef _TCPDELAYEDACKRELIABILITYCONTEXT_H
#define	_TCPDELAYEDACKRELIABILITYCONTEXT_H

#include "contexts/TCPTahoeReliabilityContext.h"
#include "states/tcp-ap/TCPDelayedACKReliabilityState.h"

class TCPDelayedACKReliabilityContext : public TCPTahoeReliabilityContext {
public:
    TCPDelayedACKReliabilityContext(u_int32_t iss);
    virtual ~TCPDelayedACKReliabilityContext();

        TimeoutEvent* get_ack_timeout_event();
    void set_ack_timeout_event(TimeoutEvent* e);

    u_int16_t get_delay_count();
    void set_delay_count(u_int16_t count);

    double get_delay_timeout_interval();
    void set_delay_timeout_interval(double count);

private:

    u_int16_t delay_count_;

    double delay_timeout_interval_;

    TimeoutEvent* ack_timer_;
};

#endif	/* _TCPDELAYEDACKRELIABILITYCONTEXT_H */

