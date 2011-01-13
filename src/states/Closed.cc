#include "states/Closed.h"

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
    cout << "Closed: Connect on socket: " << s->get_socket() << endl;
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    cmc->set_connection_type(ACTIVE);

    ConnectEvent* event;
    cout << "Closed::connect(), local AddressPort: " << s->get_local_address_port()->to_s() << endl;

    unsigned char* data = (unsigned char*) "";
    TCPPacket* p = new TCPPacket(s->get_local_address_port(), new AddressPort(remote), data, 0);
    p->set_tcp_syn(true);
    SendPacketEvent* e = new SendPacketEvent(s->get_socket(), p);
    Dispatcher::instance().enqueue(e);
    cout << "Enqueued: " << type_name(*e) << endl;

    cmc->set_state(new SynSent());
}

void Closed::listen(Context* c, Socket* s, int back_log) {
    cout << "Closed: Listen on socket: " << s->get_socket() << endl;
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    cmc->set_connection_type(PASSIVE);

    // TODO: Do anything with the Socket?
    cmc->set_back_log(back_log);
    cmc->set_state(new Listen());
}
