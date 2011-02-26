#include "states/SynReceived.h"
#include "events/ConnectionEstablishedEvent.h"
#include "events/NetworkReceivePacketEvent.h"

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

    if (packet->is_tcp_syn() && !packet->is_tcp_ack()) {
        cmc->set_state(new Accept());
        Event* e = new NetworkReceivePacketEvent(s, p);
        Dispatcher::instance().enqueue(e);
        return;
    }

    if (packet->is_tcp_ack() && ! packet->is_tcp_syn()) {
        cout << "SynReceived::receive_packet(): Packet is ACK" << endl;

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
