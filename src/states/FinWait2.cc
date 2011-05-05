#include "states/FinWait2.h"

FinWait2::FinWait2() : State() {

}

FinWait2::~FinWait2() {

}

void FinWait2::state_enter(Context* c) {
//    cout << "FinWait2::enter()" << endl;
}

void FinWait2::state_exit(Context* c) {
//    cout << "FinWait2::exit()" << endl;
}

void FinWait2::state_receive_packet(Context* c, NetworkReceivePacketEvent* e) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    TCPPacket* p = (TCPPacket*) e->get_packet();
    Socket* s = e->get_socket();

    if (p->is_tcp_fin()) {
        unsigned char* data = (unsigned char*) "";
        AddressPort* destination = s->get_remote_address_port();
        AddressPort* source = s->get_local_address_port();

        TCPPacket* response = new TCPPacket();
        response->insert_tcp_header_option(new TCPTimestampOption());
        response->set_ip_destination_address_s(destination->get_address());
        response->set_ip_source_address_s(source->get_address());

        response->set_destination_port(destination->get_port());
        response->set_source_port(source->get_port());

        response->set_data(data, 0);

        SendPacketEvent* event = new SendPacketEvent(s, response);
        Dispatcher::instance().enqueue(event);

        cmc->set_state(new TimeWait());
    }
}

bool FinWait2::state_can_receive(Context* c, Socket* s) {
    return true;
}