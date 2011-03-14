#include "contexts/CongestionControlContext.h"

CongestionControlContext::CongestionControlContext() : Context(), can_send_(true) {
    set_state(new SlowStart());
}

CongestionControlContext::~CongestionControlContext() {

}

bool CongestionControlContext::can_send_data() {
    return can_send_;
}

void CongestionControlContext::set_can_send_data(bool can_send) {
    can_send_ = can_send;
}

SendEvent* CongestionControlContext::get_buffered_send_event() {
    return buffered_send_event_;
}

void CongestionControlContext::set_buffered_send_event(SendEvent* e) {
    buffered_send_event_ = e;
}