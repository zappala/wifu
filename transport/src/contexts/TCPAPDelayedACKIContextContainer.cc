/* 
 * File:   TCPAPDelayedACKIContextContainer.cc
 * Author: leer
 * 
 * Created on July 6, 2011, 2:08 PM
 */

#include "TCPAPDelayedACKIContextContainer.h"

TCPAPDelayedACKIContextContainer::TCPAPDelayedACKIContextContainer() : TCPAPIContextContainer() {
    u_int32_t iss = 1;
    set_reliability(new TCPDelayedACKReliabilityContext(iss));
}

TCPAPDelayedACKIContextContainer::~TCPAPDelayedACKIContextContainer() {
}

