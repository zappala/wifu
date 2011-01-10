#include "../headers/states/ReliabilityState.h"

ReliabilityState::ReliabilityState() {}

ReliabilityState::~ReliabilityState() {}

void ReliabilityState::receive(Context* c, Socket* s, Packet* p) {

}

void ReliabilityState::connect(Context* c, Socket* s, AddressPort& remote) {
    //cout << "Reliability Connecting to: " << remote << endl;
}

void ReliabilityState::enter(Context* c) {
    enter_state("ReliabilityState");
}

void ReliabilityState::exit(Context* c) {
    leave_state("ReliabilityState");
}