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

void Closed::connect(Context* c, Socket* s, AddressPort& remote) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;

    unsigned char* data = (unsigned char*) "";
    TCPPacket* p = new TCPPacket(s->get_local_address_port(), new AddressPort(remote), data, 0);
    p->set_tcp_syn(true);
    Dispatcher::instance().enqueue(new SendPacketEvent(s->get_socket(), p));

    cmc->set_state(new SynSent());
}

void Closed::listen(Context* c, Socket* s, int back_log) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;

    // TODO: Do anything with the Socket?
    cmc->set_back_log(back_log);
    cmc->set_state(new Listen());
}
