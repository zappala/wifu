#include "contexts/SimpleTCPCongestionControlContext.h"

SimpleTCPCongestionControlContext::SimpleTCPCongestionControlContext() : Context() {
    set_state(new SimpleTCPCongestionControl());
}

SimpleTCPCongestionControlContext::~SimpleTCPCongestionControlContext() {

}

int SimpleTCPCongestionControlContext::get_num_outstanding() {
    return outstanding_;
}

void SimpleTCPCongestionControlContext::set_num_outstanding(int outstanding) {
    outstanding_ = outstanding;
}

u_int32_t SimpleTCPCongestionControlContext::get_last_sent_sequence_number() {
    return last_sent_sequence_number_;
}

void SimpleTCPCongestionControlContext::set_last_sent_sequence_number(u_int32_t last_sent_sequence_number) {
    last_sent_sequence_number_ = last_sent_sequence_number;
}