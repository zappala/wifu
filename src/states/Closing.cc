#include "states/Closing.h"

Closing::Closing() {

}

Closing::~Closing() {

}

void Closing::enter(Context* c) {

}

void Closing::exit(Context* c) {

}

void Closing::receive_packet(Context* c, NetworkReceivePacketEvent* e) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    TCPPacket* p = (TCPPacket*) e->get_packet();
    Socket* s = e->get_socket();

    if(p->is_tcp_ack()) {
        cmc->set_state(new TimeWait());
    }
}