#include "states/SynReceived.h"


SynReceived::SynReceived() {

}

SynReceived::~SynReceived() {

}

void SynReceived::enter(Context* c) {

}

void SynReceived::exit(Context* c) {

}

void SynReceived::receive(Context* c, Socket* s, Packet* p) {
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;

    // TODO: Ensure we receive ACK of SYN
    cmc->set_state(new Established());
}
