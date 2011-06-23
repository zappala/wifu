#include "states/SlowStart.h"

SlowStart::SlowStart() : TCPTahoeBaseCongestionControl() {

}

SlowStart::~SlowStart() {

}

void SlowStart::state_enter(Context* c) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
    
    ccc->set_cwnd(ccc->get_mss());
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
    ccc->set_ssthreshold(max((u_int32_t) (ccc->get_max_allowed_to_send() / 2), (u_int32_t) ccc->get_mss()));
    ccc->set_cwnd(ccc->get_mss());
}
