#include "states/SynSent.h"

SynSent::SynSent() {

}

SynSent::~SynSent() {

}

void SynSent::state_enter(Context* c) {

}

void SynSent::state_exit(Context* c) {

}

void SynSent::state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    TCPPacket* packet = (TCPPacket*) e->get_packet();


    if (packet->is_tcp_syn() && packet->is_tcp_ack()) {
        unsigned char* data = (unsigned char*) "";
        AddressPort* destination = packet->get_source_address_port();
        AddressPort* source = packet->get_dest_address_port();

        TCPPacket* response = new TCPPacket();
        response->insert_tcp_header_option(new TCPTimestampOption());
        response->set_ip_destination_address_s(destination->get_address());
        response->set_ip_source_address_s(source->get_address());

        response->set_destination_port(destination->get_port());
        response->set_source_port(source->get_port());
        
        response->set_data(data, 0);
        response->set_tcp_ack(true);

        SendPacketEvent* event = new SendPacketEvent(e->get_socket(), response);
        q->enqueue(event);

        cmc->set_state(new Established());
        return;
    }
}
