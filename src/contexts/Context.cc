#include "contexts/Context.h"

Context::Context() : IContext(), current_(0) {}

Context::~Context() {}

void Context::set_state(State* s) {
    // TODO: do we need a global sem to ensure that no one is caught without a context?
    if (current_) {
        current_->exit(this);
    }

    delete current_;
    current_ = s;

    if (current_) {
        current_->enter(this);
    }
}

State* Context::get_state() {
    return current_;
}


State::State() {}

State::~State() {}

void State::enter(Context* c) {}

void State::exit(Context* c) {}

void State::socket(Context* c, Socket* s) {}

void State::bind(Context* c, Socket* s, AddressPort* ap) {}

void State::listen(Context* c, Socket* s, int back_log) {}

void State::accept(Context* c, AcceptEvent* e) {}

void State::new_connection_established(Context* c, Socket* s) {}

void State::receive_packet(Context* c, Socket* s, WiFuPacket* p) {}

void State::timer_fired(Context* c, TimerFiredEvent* e) {}

void State::connect(Context* c, ConnectEvent* e) {}

void State::close(Context* c) {}

void State::send_packet(Context* c, Socket* s, WiFuPacket* p) {}

void State::resend_packet(Context*, Socket*, WiFuPacket*) {}

void State::enter_state(string state) {
    //cout << "Entering " << state << " State" << endl;
}

void State::leave_state(string state) {
    //cout << "Leaving " << state << " State" << endl;
}
