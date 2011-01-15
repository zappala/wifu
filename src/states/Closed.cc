#include "states/Closed.h"
#include "events/ConnectionEstablishedEvent.h"

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
    Socket* s = e->get_socket();
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    cmc->set_connection_type(ACTIVE_OPEN);
    cmc->set_connect_event(e);


    unsigned char* data = (unsigned char*) "";
    TCPPacket* p = new TCPPacket(s->get_local_address_port(), e->get_destination(), data, 0);
    p->set_tcp_syn(true);
    SendPacketEvent* event = new SendPacketEvent(s, p);
    Dispatcher::instance().enqueue(event);

    cmc->set_state(new SynSent());
}

void Closed::listen(Context* c, Socket* s, int back_log) {
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
