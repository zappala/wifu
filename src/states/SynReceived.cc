#include "states/SynReceived.h"


SynReceived::SynReceived() {

}

SynReceived::~SynReceived() {

}

void SynReceived::enter(Context* c) {

}

void SynReceived::exit(Context* c) {

}

void SynReceived::receive(Context* c, Socket* s, Packet* p) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    TCPPacket* packet = (TCPPacket*) p;

    assert(packet->is_tcp_ack());

    if(packet->is_tcp_ack()) {
        cout << "SynReceived::receive(), processing ACK" << endl;
        cmc->set_state(new Established());
        return;
    }

    assert(false);
}
