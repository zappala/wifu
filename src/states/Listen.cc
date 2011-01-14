#include "states/Listen.h"

Listen::Listen() {

}

Listen::~Listen() {

}

void Listen::enter(Context* c) {

}

void Listen::exit(Context* c) {

}

void Listen::accept(Context* c, Socket* s) {
    // TODO: set up the back log with a Semaphore.
    // Wait here

    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c;

    // as an example:
    // cmc->backlog.wait()

    c->set_state(new Accept());
}
