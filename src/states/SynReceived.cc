#include "states/SynReceived.h"
#include "events/ConnectionEstablishedEvent.h"

SynReceived::SynReceived() {

}

SynReceived::~SynReceived() {

}

void SynReceived::enter(Context* c) {
    cout << "Entering SynReceived State" << endl;
}

void SynReceived::exit(Context* c) {

}

void SynReceived::receive_packet(Context* c, Socket* s, WiFuPacket* p) {
    cout << "SynReceived::receive_packet()" << endl;
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    TCPPacket* packet = (TCPPacket*) p;

    assert(packet->is_tcp_ack());

    if (packet->is_tcp_ack()) {

        Socket* new_socket = new Socket(s->get_domain(),
                                            s->get_type(),
                                            s->get_protocol(),
                                            s->get_local_address_port(),
                                            packet->get_source_address_port());

        ConnectionEstablishedEvent* e = new ConnectionEstablishedEvent(cmc->get_accept_event(), new_socket);
        Dispatcher::instance().enqueue(e);
        cmc->set_state(new Listen());

        return;
    }

    assert(false);
}
