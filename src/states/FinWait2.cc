#include "states/FinWait2.h"

FinWait2::FinWait2() : State() {

}

FinWait2::~FinWait2() {

}

void FinWait2::enter(Context* c) {

}

void FinWait2::exit(Context* c) {

}

void FinWait2::receive_packet(Context* c, NetworkReceivePacketEvent* e) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    TCPPacket* p = (TCPPacket*) e->get_packet();
    Socket* s = e->get_socket();
}
