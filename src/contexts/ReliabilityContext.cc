#include "contexts/ReliabilityContext.h"

ReliabilityContext::ReliabilityContext() : Context(), seq_number_(1), ack_number_(0) {
    set_state(new ReliabilityState());
}

ReliabilityContext::~ReliabilityContext() {

}

void ReliabilityContext::set_seq_number(u_int32_t value) {
    seq_number_ = value;
}

u_int32_t ReliabilityContext::get_seq_number() {
    return seq_number_;
}

void ReliabilityContext::set_ack_number(u_int32_t value) {
    ack_number_ = value;
}

u_int32_t ReliabilityContext::get_ack_number() {
    return ack_number_;
}