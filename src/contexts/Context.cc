#include "contexts/Context.h"

Context::Context() : IContext(), current_(0) {
}

Context::~Context() {
}

void Context::set_state(State* s) {
    // TODO: do we need a global sem to ensure that no one is caught without a context?
    if (current_) {
        current_->state_exit(this);
    }

    delete current_;
    current_ = s;

    if (current_) {
        current_->state_enter(this);
    }
}

State* Context::get_state() {
    return current_;
}

void Context::icontext_socket(SocketEvent* e) {
    get_state()->state_socket(this, e);
}

void Context::icontext_bind(BindEvent* e) {
    get_state()->state_bind(this, e);
}

void Context::icontext_listen(ListenEvent* e) {
    get_state()->state_listen(this, e);
}

void Context::icontext_connect(ConnectEvent* e) {
    get_state()->state_connect(this, e);
}

void Context::icontext_accept(AcceptEvent* e) {
    get_state()->state_accept(this, e);
}

void Context::icontext_new_connection_established(ConnectionEstablishedEvent* e) {
    get_state()->state_new_connection_established(this, e);
}

void Context::icontext_new_conneciton_initiated(ConnectionInitiatedEvent* e) {
    get_state()->state_new_connection_initiated(this, e);
}

void Context::icontext_close(CloseEvent* e) {
    get_state()->state_close(this, e);
}

void Context::icontext_receive_packet(NetworkReceivePacketEvent* e) {
    get_state()->state_receive_packet(this, e);
}

void Context::icontext_send_packet(SendPacketEvent* e) {
    get_state()->state_send_packet(this, e);
}

void Context::icontext_timer_fired_event(TimerFiredEvent* e) {
    get_state()->state_timer_fired(this, e);
}

void Context::icontext_resend_packet(ResendPacketEvent* e) {
    get_state()->state_resend_packet(this, e);
}

void Context::icontext_send(SendEvent* e) {
    get_state()->state_send(this, e);
}

bool Context::icontext_can_send(Socket* s) {
    return get_state()->state_can_send(this, s);
}

bool Context::icontext_can_receive(Socket* s) {
    return get_state()->state_can_receive(this, s);
}

void Context::icontext_receive(ReceiveEvent* e) {
    get_state()->state_receive(this, e);
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
    get_state()->state_delete_socket(this, e);
}
