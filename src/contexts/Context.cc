#include "contexts/Context.h"

Context::Context() : IContext(), current_(0) {
}

Context::~Context() {
}

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

void Context::socket(SocketEvent* e) {
    get_state()->socket(this, e);
}

void Context::bind(BindEvent* e) {
    get_state()->bind(this, e);
}

void Context::listen(ListenEvent* e) {
    get_state()->listen(this, e);
}

void Context::connect(ConnectEvent* e) {
    get_state()->connect(this, e);
}

void Context::accept(AcceptEvent* e) {
    get_state()->accept(this, e);
}

void Context::new_connection_established(ConnectionEstablishedEvent* e) {
    get_state()->new_connection_established(this, e);
}

void Context::new_conneciton_initiated(ConnectionInitiatedEvent* e) {
    get_state()->new_connection_initiated(this, e);
}

void Context::icontext_close(CloseEvent* e) {
    get_state()->state_close(this, e);
}

void Context::receive_packet(NetworkReceivePacketEvent* e) {
    get_state()->receive_packet(this, e);
}

void Context::send_packet(SendPacketEvent* e) {
    get_state()->send_packet(this, e);
}

void Context::timer_fired_event(TimerFiredEvent* e) {
    get_state()->timer_fired(this, e);
}

void Context::resend_packet(ResendPacketEvent* e) {
    get_state()->resend_packet(this, e);
}

void Context::send_to(SendEvent* e) {
    get_state()->send_to(this, e);
}

bool Context::icontext_can_send(Socket* s) {
    return get_state()->state_can_send(this, s);
}

bool Context::icontext_can_receive(Socket* s) {
    return get_state()->state_can_receive(this, s);
}

void Context::receive_from(ReceiveEvent* e) {
    get_state()->receive_from(this, e);
}

void Context::icontext_receive_buffer_not_empty(ReceiveBufferNotEmptyEvent* e) {
    get_state()->state_receive_buffer_not_empty(this, e);
}

void Context::icontext_receive_buffer_not_full(ReceiveBufferNotFullEvent* e) {
    get_state()->state_receive_buffer_not_full(this, e);
}

void Context::icontext_send_buffer_not_empty(SendBufferNotEmptyEvent* e) {
    get_state()->state_send_buffer_not_empty(this, e);
}

void Context::icontext_send_buffer_not_full(SendBufferNotFullEvent* e) {
    get_state()->state_send_buffer_not_full(this, e);
}

void Context::icontext_delete_socket(DeleteSocketEvent* e) {
    
}
