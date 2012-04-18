/* 
 * File:   TCPAPRateLimiterContext.h
 * Author: leer
 *
 * Created on July 1, 2011, 5:09 PM
 */

#ifndef _TCPAPRATELIMITERCONTEXT_H
#define	_TCPAPRATELIMITERCONTEXT_H

#include <list>
#include <map>
#include "Context.h"
#include "states/tcp-ap/TCPAPSendRateLimiter.h"

class TCPAPRateLimiterContext : public Context{
public:
    TCPAPRateLimiterContext();
    virtual ~TCPAPRateLimiterContext();

    double get_fhd_hat();
    void set_fhd_hat(double fhdhat);

    double get_alpha();
    void set_alpha(double alpha);

    u_int16_t get_history_size();
    void set_history_size(u_int16_t size);

    double get_avg_rtt();
    void set_avg_rtt(double avg);

    u_int32_t get_bandwidth();
    void set_bandwidth(u_int32_t bandwidth);

    list<double, gc_allocator<double> >* get_history();

    u_int32_t get_echo_reply();
    void set_echo_reply(u_int32_t echo_reply);

    u_int16_t get_n_hop_factor();
    void set_n_hop_factor(u_int16_t factor);

    u_int32_t get_fixed_rate();
    void set_fixed_rate(u_int32_t rate);

    map<u_int32_t, u_int32_t, std::less<u_int32_t>, gc_allocator<std::pair<u_int32_t, u_int32_t> > >* get_packet_map();

private:
    //TODO: Move these into the new AP rate limiter?
    double fhd_hat_;
    double alpha_;
    double history_size_;
    list<double, gc_allocator<double> > history_;
    double avg_rtt_;
    u_int32_t bandwidth_;
    u_int32_t echo_reply_;
    u_int16_t n_hop_factor_;
    u_int32_t fixed_rate_;

    //Map sequence numbers to sent packet sizes
    map<u_int32_t, u_int32_t, std::less<u_int32_t>, gc_allocator<std::pair<u_int32_t, u_int32_t> > > sent_sizes_;

    //Here's how to get the time to receiver from the packet header options:
    //TCPTimestampOption* ts = (TCPTimestampOption*) p->get_option(TCPOPT_TIMESTAMP);;
    //And this is how Tahoe reliability figures the RTT:
    //double rtt = Utils::get_current_time_microseconds_32() - ts->get_echo_reply();
};

#endif	/* _TCPAPRATELIMITERCONTEXT_H */

