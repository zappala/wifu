/* 
 * File:   TCPAPIContextContainer.cc
 * Author: leer
 * 
 * Created on July 5, 2011, 2:49 PM
 */

#include "TCPAPIContextContainer.h"

TCPAPIContextContainer::TCPAPIContextContainer() : TCPTahoeIContextContainer(){
    set_rate_limiter(new TCPAPRateLimiterContext());
    TCPAPRateLimiterContext* rlc = (TCPAPRateLimiterContext*) get_rate_limiter();
    rlc->set_alpha(0.7);
    rlc->set_history_size(50);
    rlc->set_bandwidth(24000000);
}

TCPAPIContextContainer::~TCPAPIContextContainer() {
}

IContext* TCPAPIContextContainer::get_rate_limiter() {
    return rate_limiter_;
}

void TCPAPIContextContainer::set_rate_limiter(IContext* c) {
    rate_limiter_ = c;
}
