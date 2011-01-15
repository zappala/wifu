#include "states/Accept.h"

Accept::Accept() {

}

Accept::~Accept() {

}

void Accept::enter(Context* c) {

}

void Accept::exit(Context* c) {

}

void Accept::receive(Context* c, Socket* s, Packet* p) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    TCPPacket* packet = (TCPPacket*) p;

    assert(packet->is_tcp_syn());

    if(packet->is_tcp_syn()) {
        unsigned char* data = (unsigned char*) "";
        AddressPort* source = packet->get_destination();
        AddressPort* destination = packet->get_source();

        TCPPacket* response = new TCPPacket(source, destination, data, 0);
        response->set_tcp_syn(true);
        response->set_tcp_ack(true);

        SendPacketEvent* e = new SendPacketEvent(s, response);
        Dispatcher::instance().enqueue(e);

        cmc->set_state(new SynReceived());
        return;
    }

    // Should never receive a different type of packet in the Listent State
    assert(false);


}
