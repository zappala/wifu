#include "../headers/states/SynSent.h"

SynSent::SynSent() {

}

SynSent::~SynSent() {

}

void SynSent::enter(Context* c) {

}

void SynSent::exit(Context* c) {

}

void SynSent::receive(Context* c, Socket* s, Packet* p) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;

    // TODO: Ensure we receive a SYN, ACK
    // TODO: Send an ACK
    cmc->set_state(new Established());
}
