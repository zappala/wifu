#include "states/CongestionAvoidance.h"

CongestionAvoidance::CongestionAvoidance() {

}

CongestionAvoidance::~CongestionAvoidance() {

}

void CongestionAvoidance::set_cwnd(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;

    u_int32_t mss = ccc->get_mss();
    u_int32_t cwnd = ccc->get_cwnd();

    ccc->set_cwnd(cwnd + (mss * mss / cwnd));
}

void CongestionAvoidance::resend(Context* c, QueueProcessor<Event*>* q, ResendPacketEvent* e) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
    
    ccc->set_ssthreshold(max((u_int32_t) (ccc->get_num_outstanding() / 2), (u_int32_t) ccc->get_mss() * 2));
    ccc->set_state(new SlowStart());
    ccc->set_cwnd(ccc->get_mss());
}