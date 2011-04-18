#include "states/LastAck.h"

LastAck::LastAck() {

}

LastAck::~LastAck() {

}

void LastAck::enter(Context* c) {

}

void LastAck::exit(Context* c) {

}

void LastAck::receive_packet(Context* c, NetworkReceivePacketEvent* e) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    TCPPacket* p = (TCPPacket*) e->get_packet();
    Socket* s = e->get_socket();
}
