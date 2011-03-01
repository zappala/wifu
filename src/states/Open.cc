#include "states/Open.h"

Open::Open() : State() {

}

Open::~Open() {

}

void Open::enter(Context* c) {
    enter_state("Open");
}

void Open::exit(Context* c) {
    leave_state("Open");
}

void Open::close(Context* c) {
    //cout << "Open -> Close " << endl;
    c->set_state(new Closed());
}

