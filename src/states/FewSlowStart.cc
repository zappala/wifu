/* 
 * File:   FewSlowStart.cc
 * Author: leer
 * 
 * Created on June 28, 2011, 2:36 PM
 */

#include "states/FewSlowStart.h"

FewSlowStart::FewSlowStart() : TCPTahoeBaseCongestionControl() {

}

FewSlowStart::~FewSlowStart() {

}

void FewSlowStart::state_enter(Context* c) {
    TCPFewCongestionControlContext* ccc = (TCPFewCongestionControlContext*) c;

    ccc->set_cwnd(ccc->get_mss());
    //ccc->set_frac_cwnd((double)ccc->get_cwnd());
}

void FewSlowStart::set_cwnd(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    TCPFewCongestionControlContext* ccc = (TCPFewCongestionControlContext*) c;
    ccc->set_cwnd(ccc->get_cwnd() + (u_int32_t) ccc->get_mss());

    if (ccc->get_cwnd() > (u_int32_t) ccc->get_ssthreshold()) {
        ccc->set_state(new FewCongestionAvoidance());
    }
}

void FewSlowStart::resend(Context* c, QueueProcessor<Event*>* q, ResendPacketEvent* e) {
    TCPFewCongestionControlContext* ccc = (TCPFewCongestionControlContext*) c;
    //cout << "FewSlowStart::resend() - LOSS DETECTED\n";
    ccc->set_ssthreshold(max((u_int32_t) (ccc->get_max_allowed_to_send() / 2), (u_int32_t) ccc->get_mss()));
    ccc->set_cwnd(ccc->get_mss());
    //ccc->set_frac_cwnd((double)ccc->get_cwnd());
}

//void FewSlowStart::state_enter(Context* c) {
//    TCPFewCongestionControlContext* ccc = (TCPFewCongestionControlContext*) c;
//
//    ccc->set_cwnd(ccc->get_mss());
//}
//
//void FewSlowStart::set_cwnd(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
//    TCPFewCongestionControlContext* ccc = (TCPFewCongestionControlContext*) c;
//    ccc->set_cwnd(ccc->get_cwnd() + (u_int32_t) ccc->get_mss());
//
//    if (ccc->get_cwnd() > (u_int32_t) ccc->get_ssthreshold()) {
//        ccc->set_state(new CongestionAvoidance());
//    }
//}
//
//void FewSlowStart::resend(Context* c, QueueProcessor<Event*>* q, ResendPacketEvent* e) {
//    TCPFewCongestionControlContext* ccc = (TCPFewCongestionControlContext*) c;
//    ccc->set_ssthreshold(max((u_int32_t) (ccc->get_max_allowed_to_send() / 2), (u_int32_t) ccc->get_mss()));
//    ccc->set_cwnd(ccc->get_mss());
//}