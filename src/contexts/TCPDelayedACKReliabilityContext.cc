/* 
 * File:   TCPDelayedACKReliabilityContext.cc
 * Author: leer
 * 
 * Created on June 17, 2011, 2:09 PM
 */

#include "contexts/TCPDelayedACKReliabilityContext.h"

TCPDelayedACKReliabilityContext::TCPDelayedACKReliabilityContext(u_int32_t iss) : TCPTahoeReliabilityContext(iss), delay_timeout_interval_(INITAL_DELAY_TIMEOUT_INTERVAL), delay_count_(DEFAULT_DELAY_COUNT) {
    set_state(new TCPDelayedACKReliabilityState());
}

TCPDelayedACKReliabilityContext::~TCPDelayedACKReliabilityContext() {

}

TimeoutEvent* TCPDelayedACKReliabilityContext::get_ack_timeout_event() {
    return ack_timer_;
}

void TCPDelayedACKReliabilityContext::set_ack_timeout_event(TimeoutEvent* e) {
    ack_timer_ = e;
}

u_int16_t TCPDelayedACKReliabilityContext::get_delay_count(){
    return delay_count_;
}

void TCPDelayedACKReliabilityContext::set_delay_count(u_int16_t count) {
    delay_count_ = count;
}

double TCPDelayedACKReliabilityContext::get_delay_timeout_interval(){
    return delay_timeout_interval_;
}

void TCPDelayedACKReliabilityContext::set_delay_timeout_interval(double seconds) {
    delay_timeout_interval_ = seconds;
}
