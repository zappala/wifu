#include "states/SynReceived.h"

SynReceived::SynReceived() {

}

SynReceived::~SynReceived() {

}

void SynReceived::state_enter(Context* c) {
//    cout << "Entering SynReceived State" << endl;
}

void SynReceived::state_exit(Context* c) {

}

void SynReceived::state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
//    cout << "SynReceived::receive_packet()" << endl;
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    TCPPacket* packet = (TCPPacket*) e->get_packet();
    Socket* s = e->get_socket();

    if (packet->is_tcp_ack()) {
        ConnectionEstablishedEvent* event = new ConnectionEstablishedEvent(cmc->get_accept_socket(), s);
        q->enqueue(event);
        c->set_state(new Established());
        return;
    }
}