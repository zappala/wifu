#include "states/CloseWait.h"

CloseWait::CloseWait() {

}

CloseWait::~CloseWait() {

}

void CloseWait::enter(Context* c) {
    cout << "CloseWait::enter()" << endl;
}

void CloseWait::exit(Context* c) {
    cout << "CloseWait::exit()" << endl;
}

void CloseWait::state_close(Context* c, CloseEvent* e) {
    cout << "CloseWait::state_close()" << endl;
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    Socket* s = e->get_socket();

    unsigned char* data = (unsigned char*) "";
    AddressPort* destination = s->get_remote_address_port();
    AddressPort* source = s->get_local_address_port();

    TCPPacket* response = new TCPPacket();
    response->set_ip_protocol(SIMPLE_TCP);
    response->set_ip_destination_address_s(destination->get_address());
    response->set_ip_source_address_s(source->get_address());

    response->set_destination_port(destination->get_port());
    response->set_source_port(source->get_port());

    response->set_data(data, 0);
    response->set_tcp_fin(true);

    SendPacketEvent* event = new SendPacketEvent(s, response);
    Dispatcher::instance().enqueue(event);





    cmc->set_state(new LastAck());
    return;
}
