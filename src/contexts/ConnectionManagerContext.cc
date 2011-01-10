#include "contexts/ConnectionManagerContext.h"

ConnectionManagerContext::ConnectionManagerContext() : Context() {
    set_state(new Closed());
}

// TODO: move this method (and all others that we can) into Context
void ConnectionManagerContext::listen(Socket* s, int back_log) {
    get_state()->listen(this, s, back_log);
}

void ConnectionManagerContext::connect(Socket* s, AddressPort& dest) {
    get_state()->connect(this, s, dest);
}

void ConnectionManagerContext::close() {
    get_state()->close(this);
}

void ConnectionManagerContext::receive(string& data) {
    get_state()->receive(this, data);
}

void ConnectionManagerContext::send(Socket* s, Packet* p) {
    get_state()->send(this, s, p);
}

bool ConnectionManagerContext::is_open() {
    return get_state()->is_open();
}

// Non-state methods

int ConnectionManagerContext::get_back_log() {
    return back_log_;
}

void ConnectionManagerContext::set_back_log(int back_log) {
    back_log_ = back_log;
}