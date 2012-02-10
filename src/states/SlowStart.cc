#include "states/SlowStart.h"

SlowStart::SlowStart() : TCPTahoeBaseCongestionControl() {

}

SlowStart::~SlowStart() {

}

void SlowStart::state_enter(Context* c) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;

    // RFC 3390
    ccc->set_cwnd(min(4 * ccc->get_mss(), max(2 * ccc->get_mss(), 4380)));
}

void SlowStart::set_cwnd(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
    ccc->set_cwnd(ccc->get_cwnd() + (u_int32_t) ccc->get_mss());

    if (ccc->get_cwnd() > (u_int32_t) ccc->get_ssthreshold()) {
        ccc->set_state(new CongestionAvoidance());
    }
}

void SlowStart::resend(Context* c, QueueProcessor<Event*>* q, ResendPacketEvent* e) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
//    log_INFORMATIONAL("ss num_outstanding: ", pantheios::i(ccc->get_num_outstanding()));
    ccc->set_ssthreshold(max((u_int32_t) (ccc->get_num_outstanding() / 2), (u_int32_t) ccc->get_mss() * 2));
    ccc->set_cwnd(ccc->get_mss());
}
