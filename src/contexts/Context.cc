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

void State::socket(Context* c, SocketEvent* e) {}

void State::bind(Context* c, BindEvent* e) {}

void State::listen(Context* c, ListenEvent* e) {}

void State::accept(Context* c, AcceptEvent* e) {}

void State::new_connection_established(Context* c, ConnectionEstablishedEvent* e) {}

void State::receive_packet(Context* c, NetworkReceivePacketEvent* e) {}

void State::timer_fired(Context* c, TimerFiredEvent* e) {}

void State::connect(Context* c, ConnectEvent* e) {}

void State::close(Context* c) {}

void State::send_packet(Context* c, SendPacketEvent* e) {}

void State::resend_packet(Context*, ResendPacketEvent* e) {}

void State::enter_state(string state) {
    //cout << "Entering " << state << " State" << endl;
}

void State::leave_state(string state) {
    //cout << "Leaving " << state << " State" << endl;
}
