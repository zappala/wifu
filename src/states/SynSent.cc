#include "../headers/states/SynSent.h"

SynSent::SynSent() {

}

SynSent::~SynSent() {

}

void SynSent::enter(Context* c) {

}

void SynSent::exit(Context* c) {

}

void SynSent::receive(Context* c, Socket* s, Packet* p) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    TCPPacket* packet = (TCPPacket*) p;
    cout << "SynSent::receive()" << endl;

    assert(packet->is_tcp_syn());
    assert(packet->is_tcp_ack());

    if (packet->is_tcp_syn() && packet->is_tcp_ack()) {
        cout << "SynSent::receive(), SYN ACK received" << endl;

        unsigned char* data = (unsigned char*) "";
        AddressPort* source = packet->get_destination();
        AddressPort* destination = packet->get_source();

        TCPPacket* response = new TCPPacket(source, destination, data, 0);
        response->set_tcp_ack(true);

        SendPacketEvent* e = new SendPacketEvent(s, response);
        Dispatcher::instance().enqueue(e);
        cout << "SynSent::receive(), Dispatcher Enqueue: " << type_name(*e) << endl;

        cmc->set_state(new Established());
        return;

    }

    // TODO: Ensure we receive a SYN, ACK
    // TODO: Send an ACK
    cmc->set_state(new Established());
}
