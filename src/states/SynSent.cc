#include "../headers/states/SynSent.h"

SynSent::SynSent() {

}

SynSent::~SynSent() {

}

void SynSent::enter(Context* c) {

}

void SynSent::exit(Context* c) {

}

void SynSent::receive(Context* c, Socket* s, WiFuPacket* p) {
    cout << "In SynSent::receive()" << endl;
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    TCPPacket* packet = new TCPPacket(*p);

    assert(packet->is_tcp_syn());
    assert(packet->is_tcp_ack());

    if (packet->is_tcp_syn() && packet->is_tcp_ack()) {

        unsigned char* data = (unsigned char*) "";
        AddressPort* source = packet->get_source_address_port();
        AddressPort* destination = packet->get_dest_address_port();

        TCPPacket* response;// = new TCPPacket(source, destination, data, 0);
        response->set_tcp_ack(true);

        SendPacketEvent* e = new SendPacketEvent(s, response);
        Dispatcher::instance().enqueue(e);

        cmc->set_state(new Established());
        return;

    }

    // TODO: Ensure we receive a SYN, ACK
    // TODO: Send an ACK
    cmc->set_state(new Established());
}
