#include "states/ReliabilityState.h"
#include "contexts/ReliabilityContext.h"
#include "events/TimerEvent.h"
#include "Dispatcher.h"

ReliabilityState::ReliabilityState() {}

ReliabilityState::~ReliabilityState() {}

void ReliabilityState::receive_packet(Context* c, Socket* s, WiFuPacket* p) {
    ReliabilityContext* rc = (ReliabilityContext*) c;
}

void ReliabilityState::enter(Context* c) {
    enter_state("ReliabilityState");
}

void ReliabilityState::exit(Context* c) {
    leave_state("ReliabilityState");
}

void ReliabilityState::send_packet(Context* c, Socket* s, WiFuPacket* p) {
    ReliabilityContext* rc = (ReliabilityContext*) c;
    cout << "In ReliabilityState::send_packet()" << endl;
    TCPPacket* packet = (TCPPacket*) p;

    rc->set_saved_packet(packet);

    //TimeoutEvent* timeout = new TimeoutEvent(s, 0, 1);
    TimeoutEvent* timeout = new TimeoutEvent(s, 1, 0);
    Dispatcher::instance().enqueue(timeout);
}

void ReliabilityState::timer_fired(Context* c, TimerFiredEvent* e) {
    ReliabilityContext* rc = (ReliabilityContext*) c;
    cout << "In ReliabilityState::timer_fired(); means timeout." << endl;

    send_packet(rc, e->get_socket(), rc->get_saved_packet());
}