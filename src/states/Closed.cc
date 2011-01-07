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

void Closed::connect(Context* c, AddressPort& remote) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;

    // TODO: send a SYN
    cmc->set_state(new SynSent());
}

void Closed::listen(Context* c, Socket* s, int back_log) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;

    // TODO: Do anything with the Socket?
    cmc->set_back_log(back_log);
    cmc->set_state(new Listen());
}
