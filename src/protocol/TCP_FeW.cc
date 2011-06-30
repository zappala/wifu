/* 
 * File:   TCP_FeW.cc
 * Author: leer
 * 
 * Created on June 28, 2011, 11:48 AM
 */

#include "protocol/TCP_FeW.h"

TCP_FeW::TCP_FeW(int protocol, IContextContainerFactory* factory) : TCPTahoe(protocol, factory) {
}

TCP_FeW& TCP_FeW::instance() {
    static TCP_FeW instance_;
    return instance_;
}

TCP_FeW::~TCP_FeW() {
}

