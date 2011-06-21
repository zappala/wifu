#include "states/CongestionAvoidance.h"

CongestionAvoidance::CongestionAvoidance() {

}

CongestionAvoidance::~CongestionAvoidance() {

}

void CongestionAvoidance::set_cwnd(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;

    u_int32_t mss = ccc->get_mss();
    u_int32_t cwnd = ccc->get_cwnd();

    cout << "CongestionAvoidance::set_cwnd, CWND: " << ccc->get_cwnd() << endl;

    ccc->set_cwnd(cwnd + (mss * mss / cwnd));
}

void CongestionAvoidance::resend(Context* c, QueueProcessor<Event*>* q, ResendPacketEvent* e) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;

    // cwnd is set upon entering slow start
    ccc->set_ssthreshold(max((u_int32_t) (ccc->get_max_allowed_to_send() / 2), (u_int32_t) ccc->get_mss() * 2));
    ccc->set_state(new SlowStart());
}