#include "states/ReliabilityState.h"

ReliabilityState::ReliabilityState() {}

ReliabilityState::~ReliabilityState() {}

void ReliabilityState::receive_packet(Context* c, Socket* s, WiFuPacket* p) {

}

void ReliabilityState::connect(Context* c, ConnectEvent* e) {
    //cout << "Reliability Connecting to: " << remote << endl;
}

void ReliabilityState::enter(Context* c) {
    enter_state("ReliabilityState");
}

void ReliabilityState::exit(Context* c) {
    leave_state("ReliabilityState");
}