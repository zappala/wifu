/* 
 * File:   TCPFewIContextContainer.cc
 * Author: leer
 * 
 * Created on June 28, 2011, 1:21 PM
 */

#include "contexts/TCPFewIContextContainer.h"
#include "contexts/TCPFewCongestionControlContext.h"

TCPFewIContextContainer::TCPFewIContextContainer() : TCPTahoeIContextContainer() {
    u_int32_t iss = 1;
    set_congestion_control(new TCPFewCongestionControlContext(iss));
    TCPFewCongestionControlContext* ccc = (TCPFewCongestionControlContext*)get_congestion_control();
    ccc->set_alpha(0.2);
}

TCPFewIContextContainer::~TCPFewIContextContainer() {
}

