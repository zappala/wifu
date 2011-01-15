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
    cout << "Closed: Connect on socket: " << s->get_socket_id() << endl;
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    cmc->set_connection_type(ACTIVE_OPEN);
    cmc->set_connect_event(e);

    cout << "Closed::connect(), local AddressPort: " << s->get_local_address_port()->to_s() << endl;

    unsigned char* data = (unsigned char*) "";
    TCPPacket* p = new TCPPacket(s->get_local_address_port(), e->get_destination(), data, 0);
    p->set_tcp_syn(true);
    SendPacketEvent* event = new SendPacketEvent(s, p);
    Dispatcher::instance().enqueue(event);
    cout << "Enqueued: " << type_name(*event) << endl;

    cmc->set_state(new SynSent());
}

void Closed::listen(Context* c, Socket* s, int back_log) {
    cout << "Closed: Listen on socket: " << s->get_socket_id() << endl;
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    cmc->set_connection_type(PASSIVE_OPEN);

    // TODO: Do anything with the Socket?
    cmc->set_back_log(back_log);
    cmc->set_state(new Listen());
}

void Closed::connection_established(Context* c, Socket* s) {
    cout << "Closed: Connection Established" << endl;
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    cmc->set_connection_type(ESTABLISHED);
    c->set_state(new Established());
}
