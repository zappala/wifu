#include "states/Closed.h"
#include "events/ConnectionEstablishedEvent.h"
#include "SourceGetter.h"

Closed::Closed() : State() {

}

Closed::~Closed() {

}

void Closed::enter(Context* c) {
    enter_state("Closed");
}

void Closed::exit(Context* c) {
    leave_state("Closed");
}

void Closed::connect(Context* c, ConnectEvent* e) {
    cout << "In Closed::connect()" << endl;
    Socket* s = e->get_socket();
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    cmc->set_connection_type(ACTIVE_OPEN);
    cmc->set_connect_event(e);

    string dest_string = e->get_destination()->get_address();

    unsigned char* data = (unsigned char*) "";
    TCPPacket* p = new TCPPacket();

    int port = s->get_local_address_port()->get_port();
    string address = SourceGetter::instance().get_source_address(dest_string);
    
    AddressPort* source = new AddressPort(address, port);
    AddressPort* destination = e->get_destination();
    
    s->set_local_address_port(source);
    s->set_remote_address_port(destination);

    p->set_ip_protocol(SIMPLE_TCP);
    p->set_ip_destination_address_s(destination->get_address());
    p->set_ip_source_address_s(source->get_address());

    p->set_destination_port(destination->get_port());
    p->set_source_port(source->get_port());

    p->set_tcp_syn(true);
    p->set_data(data, 0);

    SendPacketEvent* event = new SendPacketEvent(s, p);
    Dispatcher::instance().enqueue(event);

    // TODO: move this earlier so we don't send and dequeue a packet while still in this (Closed) state
    // We don't want the FSM to be in between changing states either (see set_state).
    cout << "Closed::connect(), switching to SynSent" << endl;
    cmc->set_state(new SynSent());
}

void Closed::listen(Context* c, Socket* s, int back_log) {
    cout << "Closed::listen()" << endl;
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    cmc->set_connection_type(PASSIVE_OPEN);

    // TODO: Do anything with the Socket?
    cmc->set_back_log(back_log);
    cmc->set_state(new Listen());
}

void Closed::connection_established(Context* c, Socket* s) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    cmc->set_connection_type(ESTABLISHED);
    c->set_state(new Established());
}
