#include "states/Accept.h"

Accept::Accept() {

}

Accept::~Accept() {

}

void Accept::enter(Context* c) {

}

void Accept::exit(Context* c) {

}

void Accept::receive_packet(Context* c, NetworkReceivePacketEvent* e) {
    cout << "Accept::receive_packet()" << endl;
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    TCPPacket* packet = (TCPPacket*) e->get_packet();

    if(packet->is_tcp_syn()) {
        cout << "Accept::receive_packet(): Packet is a SYN" << endl;

        




        unsigned char* data = (unsigned char*) "";
        AddressPort* source = packet->get_dest_address_port();
        AddressPort* destination = packet->get_source_address_port();

        TCPPacket* response = new TCPPacket();
        response->set_ip_protocol(SIMPLE_TCP);
        response->set_ip_destination_address_s(destination->get_address());
        response->set_ip_source_address_s(source->get_address());

        response->set_destination_port(destination->get_port());
        response->set_source_port(source->get_port());

        response->set_tcp_syn(true);
        response->set_tcp_ack(true);
        response->set_data(data, 0);

        SendPacketEvent* event = new SendPacketEvent(e->get_socket(), response);
        Dispatcher::instance().enqueue(event);

        cmc->set_state(new SynReceived());
    }
}
