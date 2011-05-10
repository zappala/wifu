/* 
 * File:   TCPTahoeReliabilityContext.h
 * Author: rbuck
 *
 * Created on May 6, 2011, 1:24 PM
 */

#ifndef TCPTAHOERELIABILITYCONTEXT_H
#define	TCPTAHOERELIABILITYCONTEXT_H

#include "Context.h"
#include "states/TCPTahoeReliabilityState.h"

#include "events/TimeoutEvent.h"

class TCPTahoeReliabilityContext : public Context {
public:
    TCPTahoeReliabilityContext();
    virtual ~TCPTahoeReliabilityContext();

    TimeoutEvent* get_timer();
    void set_timer(TimeoutEvent* e);

    long int get_rtt();
    void set_rtt(long int rtt);

    long int get_rto();
    void set_rto(long int rto);

    long int get_srtt();
    void set_srtt(long int srtt);
    
private:
    TimeoutEvent* timer_;

    // all these are in nanoseconds
    long int rto_;
    long int rtt_;
    long int srtt_;
};

#endif	/* TCPTAHOERELIABILITYCONTEXT_H */

