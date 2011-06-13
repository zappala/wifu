#include "states/CongestionAvoidance.h"

CongestionAvoidance::CongestionAvoidance() {

}

CongestionAvoidance::~CongestionAvoidance() {

}

void CongestionAvoidance::set_cwnd(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;

    u_int16_t mss = ccc->get_mss();
    u_int16_t cwnd = ccc->get_cwnd();

    ccc->set_cwnd(cwnd + (mss * mss / cwnd));
}

void CongestionAvoidance::resend(Context* c, QueueProcessor<Event*>* q, ResendPacketEvent* e) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;

    // cwnd is set upon entering slow start
    ccc->set_ssthreshold(max((u_int16_t) (ccc->get_max_allowed_to_send() / 2), ccc->get_mss()));
    ccc->set_state(new SlowStart());
}