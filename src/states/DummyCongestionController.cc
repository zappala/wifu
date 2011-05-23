/* 
 * File:   DummyCongestionController.cc
 * Author: rbuck
 * 
 * Created on May 23, 2011, 2:59 PM
 */

#include "states/DummyCongestionController.h"

DummyCongestionController::DummyCongestionController() : State() {
}

DummyCongestionController::DummyCongestionController(const DummyCongestionController& orig) : State() {
}

DummyCongestionController::~DummyCongestionController() {
}

void DummyCongestionController::state_receive(Context* c, ReceiveEvent* e) {
    send_packets(c, e);
}

void DummyCongestionController::state_send_buffer_not_empty(Context* c, SendBufferNotEmptyEvent* e) {
    send_packets(c, e);
}

void DummyCongestionController::send_packets(Context* c, Event* e) {
    Socket* s = e->get_socket();
    
}

