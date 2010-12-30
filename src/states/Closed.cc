#include "Closed.h"
#include "Open.h"

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

void Closed::connect(Context* c, string& remote) {
    cout << "Closed: Connect to: " << remote << endl;
    c->set_state(new Open());
}
