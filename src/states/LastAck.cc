#include "states/LastAck.h"

LastAck::LastAck() {

}

LastAck::~LastAck() {

}

void LastAck::state_enter(Context* c) {

}

void LastAck::state_exit(Context* c) {

}

void LastAck::state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    TCPPacket* p = (TCPPacket*) e->get_packet();
    Socket* s = e->get_socket();

    if (p->is_naked_ack()) {
        q->enqueue(new DeleteSocketEvent(s));
        cmc->set_state(new Closed());
    }
}
