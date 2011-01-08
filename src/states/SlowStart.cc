#include "../headers/states/SlowStart.h"


SlowStart::SlowStart() {

}

SlowStart::~SlowStart() {

}

void SlowStart::send(Context* c, Socket* s, Packet* p) {
    // Send immediately for now
    CongestionControlContext* ccc = (CongestionControlContext*) c;
    UDPSendPacketEvent* e = new UDPSendPacketEvent(s->get_socket(), p);
    Dispatcher::instance().enqueue(e);
}

void SlowStart::receive(Context* c, string& data) {

}

void SlowStart::enter(Context* c) {
    enter_state("SlowStart");
}

void SlowStart::exit(Context* c) {
    leave_state("SlowStart");
}


