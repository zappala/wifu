#include "Connection.h"
#include "Closed.h"


Connection::Connection() {
    setState(new Closed());
}

Connection::~Connection() {
    if(state != 0) {
        delete state;
    }
}

void Connection::setState(TCPState* newState) {
    if (state != 0) {
        state->exit(this);
    }
    delete state;
    state = newState;
    if (state != 0) {
        state->enter(this);
    }
}
