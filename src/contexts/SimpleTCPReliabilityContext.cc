#include "contexts/SimpleTCPReliabilityContext.h"

SimpleTCPReliabilityContext::SimpleTCPReliabilityContext() : Context(), seq_number_(1), ack_number_(0), timeout_event_(0) {
    set_state(new SimpleTCPReliability());
}

SimpleTCPReliabilityContext::~SimpleTCPReliabilityContext() {

}

void SimpleTCPReliabilityContext::set_seq_number(u_int32_t value) {
    seq_number_ = value;
}

u_int32_t SimpleTCPReliabilityContext::get_seq_number() {
    return seq_number_;
}

void SimpleTCPReliabilityContext::set_ack_number(u_int32_t value) {
    ack_number_ = value;
}

u_int32_t SimpleTCPReliabilityContext::get_ack_number() {
    return ack_number_;
}

void SimpleTCPReliabilityContext::set_timeout_event(TimeoutEvent* e) {
    timeout_event_ = e;
}

TimeoutEvent* SimpleTCPReliabilityContext::get_timeout_event() {
    return timeout_event_;
}
