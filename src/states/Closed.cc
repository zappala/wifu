#include "states/Closed.h"

Closed::Closed() : State() {

}

Closed::~Closed() {

}

void Closed::state_connect(Context* c, QueueProcessor<Event*>* q, ConnectEvent* e) {
//    cout << "In Closed::connect()" << endl;
    Socket* s = e->get_socket();
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    cmc->set_connection_type(ACTIVE_OPEN);
    cmc->set_connect_event(e);

    string dest_string = e->get_destination()->get_address();

    unsigned char* data = (unsigned char*) "";
    TCPPacket* p = new TCPPacket();
    p->insert_tcp_header_option(new TCPTimestampOption());

    int port = s->get_local_address_port()->get_port();
    string address = SourceGetter::instance().get_source_address(dest_string);
    
    AddressPort* source = new AddressPort(address, port);
    AddressPort* destination = e->get_destination();
    
    s->set_local_address_port(source);
    s->set_remote_address_port(destination);

    p->set_ip_destination_address_s(destination->get_address());
    p->set_ip_source_address_s(source->get_address());

    p->set_destination_port(destination->get_port());
    p->set_source_port(source->get_port());

    p->set_tcp_syn(true);
    p->set_data(data, 0);

    SendPacketEvent* event = new SendPacketEvent(s, p);
    q->enqueue(event);

    // TODO: move this earlier so we don't send and dequeue a packet while still in this (Closed) state
    // We don't want the FSM to be in between changing states either (see set_state).
//    cout << "Closed::connect(), switching to SynSent" << endl;
    cmc->set_state(new SynSent());
}

void Closed::state_listen(Context* c, QueueProcessor<Event*>* q, ListenEvent* e) {
//    cout << "Closed::listen()" << endl;
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    cmc->set_connection_type(PASSIVE_OPEN);

    // TODO: Do anything with the Socket?
    cmc->set_back_log(e->get_back_log());
    cmc->set_state(new Listen());
}

void Closed::state_new_connection_established(Context* c, QueueProcessor<Event*>* q, ConnectionEstablishedEvent* e) {
//    cout << "Closed::state_new_connection_established()" << endl;
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    cmc->set_connection_type(ESTABLISHED);
    c->set_state(new Established());
}

void Closed::state_new_connection_initiated(Context* c, QueueProcessor<Event*>* q, ConnectionInitiatedEvent* e) {
//    cout << "Closed::state_new_connection_initiated()" << endl;
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    cmc->set_connection_type(PASSIVE_OPEN);
    c->set_state(new Listen());
}

void Closed::state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    cout << "Closed::state_receive_packet()" << endl;
}
