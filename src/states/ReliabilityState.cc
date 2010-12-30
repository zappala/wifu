#include "ReliabilityState.h"

ReliabilityState::ReliabilityState() {}

ReliabilityState::~ReliabilityState() {}

void ReliabilityState::receive(Context* c, string& data) {

}

void ReliabilityState::connect(Context* c, string& remote) {
    cout << "Reliability Connecting to: " << remote << endl;
}

void ReliabilityState::enter(Context* c) {
    enter_state("ReliabilityState");
}

void ReliabilityState::exit(Context* c) {
    leave_state("ReliabilityState");
}