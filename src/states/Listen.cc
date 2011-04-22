#include "states/Listen.h"

Listen::Listen() {

}

Listen::~Listen() {

}

void Listen::state_enter(Context* c) {

}

void Listen::state_exit(Context* c) {

}

void Listen::state_accept(Context* c, AcceptEvent* e) {
    // TODO: set up the back log with a Semaphore.
    // Wait here

    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;
    cmc->set_accept_event(e);
    
    // as an example:
    // cmc->backlog.wait()

    c->set_state(new Accept());
}
