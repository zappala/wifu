/* 
 * File:   TCP_AP_DelayedACKs.cc
 * Author: leer
 * 
 * Created on July 6, 2011, 2:35 PM
 */

#include "protocol/TCP_AP_DelayedACKs.h"

TCP_AP_DelayedACKs::TCP_AP_DelayedACKs(int protocol = TCP_AP_DELAYEDACK, IContextContainerFactory* factory = new TCPAPDelayedACKIContextContainerFactory()) : TCPAP(protocol, factory) {
}

TCP_AP_DelayedACKs::~TCP_AP_DelayedACKs() {
}

TCP_AP_DelayedACKs& TCP_AP_DelayedACKs::instance() {
    static TCP_AP_DelayedACKs instance_;
    return instance_;
}