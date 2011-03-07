#include "contexts/CongestionControlContext.h"

CongestionControlContext::CongestionControlContext() : Context(), max_buffer_size_(1024 * 1024), can_send_(true) {
    set_state(new SlowStart());
}

CongestionControlContext::~CongestionControlContext() {

}

IQueue<unsigned char>& CongestionControlContext::get_queue() {
    return buffer_;
}

int CongestionControlContext::get_max_buffer_size() {
    return max_buffer_size_;
}

bool CongestionControlContext::can_send_data() {
    return can_send_;
}

void CongestionControlContext::set_can_send_data(bool can_send) {
    can_send_ = can_send;
}