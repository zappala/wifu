#include "states/ReliabilityState.h"

ReliabilityState::ReliabilityState() {
}

ReliabilityState::~ReliabilityState() {
}

void ReliabilityState::receive_packet(Context* c, Socket* s, WiFuPacket* p) {
    ReliabilityContext* rc = (ReliabilityContext*) c;
    TCPPacket* packet = (TCPPacket*) p;

    if (packet->is_tcp_ack() && (rc->get_seq_counter() == packet->get_tcp_ack_number())) {
        //cancel the timeout
        rc->set_saved_timeout(NULL);
    }

    rc->set_last_packet_received(packet);
    // TODO: remove any ACK'ed packet(s) from memory?
}

void ReliabilityState::enter(Context* c) {
    enter_state("ReliabilityState");
}

void ReliabilityState::exit(Context* c) {
    leave_state("ReliabilityState");
}

void ReliabilityState::send_packet(Context* c, Socket* s, WiFuPacket* p) {
    cout << "In ReliabilityState::send_packet()" << endl;

    ReliabilityContext* rc = (ReliabilityContext*) c;
    TCPPacket* packet = (TCPPacket*) p;

    TCPPacket* last_received = rc->get_last_packet_received();
    if(last_received != 0) {
        packet->set_tcp_ack_number(last_received->get_tcp_sequence_number() + 1);
    }

    u_int32_t seq_num = rc->get_seq_counter();
    packet->set_tcp_sequence_number(seq_num);
    rc->set_seq_counter(rc->get_seq_counter() + 1);
    
    rc->set_last_packet_sent(packet);

    if(should_set_resend_timer(packet)) {
        TimeoutEvent* timeout = new TimeoutEvent(s, 1, 0);
        rc->set_saved_timeout(timeout);
        Dispatcher::instance().enqueue(timeout);
    }
}

void ReliabilityState::timer_fired(Context* c, TimerFiredEvent* e) {
    ReliabilityContext* rc = (ReliabilityContext*) c;

    if(rc->get_saved_timeout() == NULL || rc->get_saved_timeout() != e->get_timeout_event()) {
        return;
    }

    cout << "In ReliabilityState::timer_fired(); means timeout." << endl;

    ResendPacketEvent* event = new ResendPacketEvent(e->get_socket(), rc->get_last_packet_sent());
    Dispatcher::instance().enqueue(event);
}

void ReliabilityState::resend_packet(Context* c, Socket* s, WiFuPacket* p) {
    ReliabilityContext* rc = (ReliabilityContext*) c;
    cout << "In ReliabilityState::resend_packet()" << endl;

    //TimeoutEvent* timeout = new TimeoutEvent(s, 0, 1);
    TimeoutEvent* timeout = new TimeoutEvent(s, 1, 0);
    rc->set_saved_timeout(timeout);
    Dispatcher::instance().enqueue(timeout);
}

bool ReliabilityState::should_set_resend_timer(TCPPacket* p) {
    
}