/* 
 * File:   TCPFewCongestionControlContext.cc
 * Author: leer
 * 
 * Created on June 28, 2011, 2:50 PM
 */

#include "contexts/TCPFewCongestionControlContext.h"

TCPFewCongestionControlContext::TCPFewCongestionControlContext(u_int32_t iss) : TCPTahoeCongestionControlContext(iss), alpha_(0.2){

    TCPPacket p;
    TCPTimestampOption o;
    p.insert_tcp_header_option(&o);
    set_mss(p.max_data_length());
    set_state(new FewSlowStart());
    
    //frac_cwnd_ = (double)get_mss();
    min_cwnd_size_ = (double)get_mss();
    //cout << "FYI, initial frac_cwnd is " << frac_cwnd_ << endl;
}

TCPFewCongestionControlContext::~TCPFewCongestionControlContext() {

}

double TCPFewCongestionControlContext::get_alpha() const {
    return alpha_;
}

void TCPFewCongestionControlContext::set_alpha(double alpha) {
    alpha_ = alpha;
}

//double TCPFewCongestionControlContext::get_frac_cwnd() const {
//    return frac_cwnd_;
//}
//
//void TCPFewCongestionControlContext::set_frac_cwnd(double frac_cwnd) {
//    frac_cwnd_ = frac_cwnd;
//}

u_int32_t TCPFewCongestionControlContext::get_min_cwnd_size() const {
    return min_cwnd_size_;
}

void TCPFewCongestionControlContext::set_min_cwnd_size(u_int32_t minsize) {
    min_cwnd_size_ = minsize;
}