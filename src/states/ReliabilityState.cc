#include "states/ReliabilityState.h"

ReliabilityState::ReliabilityState() {
}

ReliabilityState::~ReliabilityState() {
}

void ReliabilityState::receive_packet(Context* c, NetworkReceivePacketEvent* e) {

}

void ReliabilityState::enter(Context* c) {
    enter_state("ReliabilityState");
}

void ReliabilityState::exit(Context* c) {
    leave_state("ReliabilityState");
}

void ReliabilityState::send_packet(Context* c, SendPacketEvent* e) {

}

void ReliabilityState::timer_fired(Context* c, TimerFiredEvent* e) {

}

void ReliabilityState::resend_packet(Context* c, ResendPacketEvent* e) {
}
