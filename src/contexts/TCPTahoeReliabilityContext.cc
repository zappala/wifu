#include "contexts/TCPTahoeReliabilityContext.h"

// TOOD: this is overflowing
TCPTahoeReliabilityContext::TCPTahoeReliabilityContext() : timer_(0), rtt_(0), srtt_(0), rto_(3 * NANOSECONDS_IN_SECONDS) {
    set_state(new TCPTahoeReliabilityState());
}

TCPTahoeReliabilityContext::~TCPTahoeReliabilityContext() {

}

TimeoutEvent* TCPTahoeReliabilityContext::get_timer() {
    return timer_;
}

void TCPTahoeReliabilityContext::set_timer(TimeoutEvent* e) {
    timer_ = e;
}

long int TCPTahoeReliabilityContext::get_rtt() {
    return rtt_;
}

void TCPTahoeReliabilityContext::set_rtt(long int rtt) {
    rtt_ = rtt;
}

long int TCPTahoeReliabilityContext::get_rto() {
    return rto_;
}

void TCPTahoeReliabilityContext::set_rto(long int rto) {
    rto_ = rto;
}

long int TCPTahoeReliabilityContext::get_srtt() {
    return srtt_;
}

void TCPTahoeReliabilityContext::set_srtt(long int srtt) {
    srtt_ = srtt;
}