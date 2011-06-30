/* 
 * File:   FewCongestionAvoidance.cc
 * Author: leer
 * 
 * Created on June 28, 2011, 2:37 PM
 */

#include "states/FewCongestionAvoidance.h"

FewCongestionAvoidance::FewCongestionAvoidance() {
}

FewCongestionAvoidance::~FewCongestionAvoidance() {
}

void FewCongestionAvoidance::set_cwnd(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    TCPFewCongestionControlContext* ccc = (TCPFewCongestionControlContext*) c;

    u_int16_t mss = ccc->get_mss();
    //double cwnd = ccc->get_frac_cwnd();
    u_int32_t cwnd = ccc->get_cwnd();

    //cout << "FewCongestionAvoidance::set_cwnd(): frac_cwnd = " << cwnd << endl;
    //TODO: make sure we have a minimum window size (probably 1 MSS for comparison with Tahoe)
    double alpha = ccc->get_alpha();
    //cout << "FewCongestionAvoidance::set_cwnd(): alpha = " << alpha << endl;
    //cout << "FewCongestionAvoidance::set_cwnd(): mss = " << mss << endl;

    cwnd = (cwnd + (mss * mss * alpha / cwnd));
//    cout << "FewCongestionAvoidance::set_cwnd(): frac_cwnd now = " << ccc->get_frac_cwnd() << endl;
//    cout << "FewCongestionAvoidance::set_cwnd(): due to increment of " << (alpha / (mss * mss / cwnd)) << endl;
//    cout << "FewCongestionAvoidance::set_cwnd(): min_cwnd_size = " << ccc->get_min_cwnd_size() << endl;
//    cout << "FewCongestionAvoidance::set_cwnd(): setting cwnd to " << max(floor(ccc->get_frac_cwnd()), ccc->get_min_cwnd_size()) << endl;
    ccc->set_cwnd(max(cwnd, ccc->get_min_cwnd_size()));
//    ccc->set_cwnd(cwnd + (mss * mss / cwnd));
}

void FewCongestionAvoidance::resend(Context* c, QueueProcessor<Event*>* q, ResendPacketEvent* e) {
    TCPFewCongestionControlContext* ccc = (TCPFewCongestionControlContext*) c;

    //cout << "FewCongestionAvoidance::resend() - LOSS DETECTED\n";

    // cwnd is set upon entering slow start
    ccc->set_ssthreshold(max((u_int32_t) (ccc->get_max_allowed_to_send() / 2), (u_int32_t) ccc->get_mss() * 2));
    ccc->set_state(new FewSlowStart());
}

//void FewCongestionAvoidance::set_cwnd(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
//    TCPFewCongestionControlContext* ccc = (TCPFewCongestionControlContext*) c;
//
//    u_int32_t mss = ccc->get_mss();
//    u_int32_t cwnd = ccc->get_cwnd();
//
//    ccc->set_cwnd(cwnd + (mss * mss / cwnd));
//}
//
//void FewCongestionAvoidance::resend(Context* c, QueueProcessor<Event*>* q, ResendPacketEvent* e) {
//    TCPFewCongestionControlContext* ccc = (TCPFewCongestionControlContext*) c;
//
//    // cwnd is set upon entering slow start
//    ccc->set_ssthreshold(max((u_int32_t) (ccc->get_max_allowed_to_send() / 2), (u_int32_t) ccc->get_mss() * 2));
//    ccc->set_state(new SlowStart());
//}