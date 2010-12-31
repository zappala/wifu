#include "states/Listen.h"


Listen::Listen() {

}

Listen::~Listen(){

}

void Listen::enter(Context* c) {

}

void Listen::exit(Context* c) {

}

void Listen::receive(Context* c, string& data) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    // TODO: ensure we receive a SYN
    // TODO: send SYN, ACK
    cmc->set_state(new SynReceived());
}
