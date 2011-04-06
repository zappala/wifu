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
    cout << "ReliabilityState::send_packet()" << endl;

    ReliabilityContext* rc = (ReliabilityContext*) c;
    Socket* s = e->get_socket();
    TCPPacket* p = (TCPPacket*) e->get_packet();

    u_int32_t seq_num = rc->get_seq_number();
    p->set_tcp_sequence_number(seq_num);
    rc->set_seq_number(seq_num + 1);

    u_int32_t ack_num = rc->get_ack_number();
    if(ack_num != 0) {
        p->set_tcp_ack(true);
        p->set_tcp_ack_number(ack_num);
    }

    if(!p->is_naked_ack()) {
        SimpleTCPCache* cache = (SimpleTCPCache*) CacheMap::instance().get(s);
        cache->save_packet(p);
        Dispatcher::instance().enqueue(new TimeoutEvent(s, 1, 0));
    }
}

void ReliabilityState::timer_fired(Context* c, TimerFiredEvent* e) {

}

void ReliabilityState::resend_packet(Context* c, ResendPacketEvent* e) {
}
