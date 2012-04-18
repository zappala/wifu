#include "states/Closing.h"

Closing::Closing() {

}

Closing::~Closing() {

}

void Closing::state_enter(Context* c) {

}

void Closing::state_exit(Context* c) {

}

void Closing::state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    TCPPacket* p = (TCPPacket*) e->get_packet();
    Socket* s = e->get_socket();

    if(p->is_tcp_ack()) {
        cmc->set_state(new TimeWait());
    }
}