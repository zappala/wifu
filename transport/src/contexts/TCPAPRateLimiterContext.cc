/* 
 * File:   TCPAPRateLimiterContext.cc
 * Author: leer
 * 
 * Created on July 1, 2011, 5:09 PM
 */

#include "contexts/TCPAPRateLimiterContext.h"

TCPAPRateLimiterContext::TCPAPRateLimiterContext() : Context(), echo_reply_(0), fhd_hat_(0), n_hop_factor_(4), fixed_rate_(-1) {
    set_state(new TCPAPSendRateLimiter());
}

TCPAPRateLimiterContext::~TCPAPRateLimiterContext() {
}

double TCPAPRateLimiterContext::get_fhd_hat() {
    return fhd_hat_;
}

void TCPAPRateLimiterContext::set_fhd_hat(double fhdhat) {
    fhd_hat_ = fhdhat;
}

double TCPAPRateLimiterContext::get_alpha() {
    return alpha_;
}

void TCPAPRateLimiterContext::set_alpha(double alpha) {
    alpha_ = alpha;
}

u_int16_t TCPAPRateLimiterContext::get_history_size() {
    return history_size_;
}

double TCPAPRateLimiterContext::get_avg_rtt() {
    return avg_rtt_;
}

void TCPAPRateLimiterContext::set_avg_rtt(double avg) {
    avg_rtt_ = avg;
}

u_int32_t TCPAPRateLimiterContext::get_bandwidth() {
    return bandwidth_;
}

void TCPAPRateLimiterContext::set_bandwidth(u_int32_t bandwidth) {
    bandwidth_ = bandwidth;
}

void TCPAPRateLimiterContext::set_history_size(u_int16_t size) {
    history_size_ = size;
}

list<double, gc_allocator<double> >* TCPAPRateLimiterContext::get_history() {
    return &history_;
}

u_int32_t TCPAPRateLimiterContext::get_echo_reply() {
    return echo_reply_;
}

void TCPAPRateLimiterContext::set_echo_reply(u_int32_t echo_reply) {
    echo_reply_ = echo_reply;
}

u_int16_t TCPAPRateLimiterContext::get_n_hop_factor() {
    return n_hop_factor_;
}

void TCPAPRateLimiterContext::set_n_hop_factor(u_int16_t factor) {
    n_hop_factor_ = factor;
}

u_int32_t TCPAPRateLimiterContext::get_fixed_rate() {
    return fixed_rate_;
}

void TCPAPRateLimiterContext::set_fixed_rate(u_int32_t rate) {
    fixed_rate_ = rate;
}

map<u_int32_t, u_int32_t, std::less<u_int32_t>, gc_allocator<std::pair<u_int32_t, u_int32_t> > >* TCPAPRateLimiterContext::get_packet_map() {
    return & sent_sizes_;
}