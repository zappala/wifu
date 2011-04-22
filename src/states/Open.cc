#include "states/Open.h"

Open::Open() : State() {

}

Open::~Open() {

}

void Open::close(Context* c) {
    //cout << "Open -> Close " << endl;
    c->set_state(new Closed());
}

