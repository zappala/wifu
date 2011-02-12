#include "states/SlowStart.h"


SlowStart::SlowStart() {

}

SlowStart::~SlowStart() {

}

void SlowStart::send_packet(Context* c, Socket* s, WiFuPacket* p) {
    // Send immediately for now
    cout << "SlowStart::send()" << endl;
    CongestionControlContext* ccc = (CongestionControlContext*) c;
}

void SlowStart::receive_packet(Context* c, Socket* s, WiFuPacket* p) {

}

void SlowStart::enter(Context* c) {
    enter_state("SlowStart");
}

void SlowStart::exit(Context* c) {
    leave_state("SlowStart");
}


