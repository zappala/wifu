#include "states/FinWait1.h"

FinWait1::FinWait1() : State() {

}

FinWait1::~FinWait1() {

}

void FinWait1::enter(Context* c) {

}

void FinWait1::exit(Context* c) {

}

void FinWait1::close(Context* c) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
}

void FinWait1::receive_packet(Context* c, NetworkReceivePacketEvent* e) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    TCPPacket* p = (TCPPacket*) e->get_packet();
    Socket* s = e->get_socket();
    
}
