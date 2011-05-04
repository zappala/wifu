#include "contexts/CongestionControlContext.h"

CongestionControlContext::CongestionControlContext() : Context() {
    set_state(new SimpleTCPCongestionControl());
}

CongestionControlContext::~CongestionControlContext() {

}

int CongestionControlContext::get_num_outstanding() {
    return outstanding_;
}

void CongestionControlContext::set_num_outstanding(int outstanding) {
    outstanding_ = outstanding;
}

u_int32_t CongestionControlContext::get_last_sent_sequence_number() {
    return last_sent_sequence_number_;
}

void CongestionControlContext::set_last_sent_sequence_number(u_int32_t last_sent_sequence_number) {
    last_sent_sequence_number_ = last_sent_sequence_number;
}
