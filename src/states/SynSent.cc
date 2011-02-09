#include "states/SynSent.h"

SynSent::SynSent() {

}

SynSent::~SynSent() {

}

void SynSent::enter(Context* c) {

}

void SynSent::exit(Context* c) {

}

void SynSent::receive_packet(Context* c, Socket* s, WiFuPacket* p) {
    cout << "SynSent::receive_packet()" << endl;
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    TCPPacket* packet = new TCPPacket(*p);

    assert(packet->is_tcp_syn());
    assert(packet->is_tcp_ack());

    if (packet->is_tcp_syn() && packet->is_tcp_ack()) {
        cout << "SynSent::receive_packet(): Packet is SYNACK" << endl;
        unsigned char* data = (unsigned char*) "";
        AddressPort* destination = packet->get_source_address_port();
        AddressPort* source = packet->get_dest_address_port();

        TCPPacket* response = new TCPPacket();
        response->set_ip_protocol(SIMPLE_TCP);
        response->set_ip_destination_address_s(destination->get_address());
        response->set_ip_source_address_s(source->get_address());

        response->set_destination_port(destination->get_port());
        response->set_source_port(source->get_port());
        
        response->set_tcp_ack(true);
        response->set_tcp_urg(true);
        response->set_data(data, 0);

        SendPacketEvent* e = new SendPacketEvent(s, response);
        Dispatcher::instance().enqueue(e);

        cmc->set_state(new Established());
        return;
    }

    assert(false);
}
