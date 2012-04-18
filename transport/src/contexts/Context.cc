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
    assert(current_);
    return current_;
}

gcstring Context::get_state_name() {
    return type_name(*get_state());
}

void Context::icontext_socket(QueueProcessor<Event*>* q, SocketEvent* e) {
    get_state()->state_socket(this, q, e);
}

void Context::icontext_bind(QueueProcessor<Event*>* q, BindEvent* e) {
    get_state()->state_bind(this, q, e);
}

void Context::icontext_listen(QueueProcessor<Event*>* q, ListenEvent* e) {
    get_state()->state_listen(this, q, e);
}

void Context::icontext_connect(QueueProcessor<Event*>* q, ConnectEvent* e) {
    get_state()->state_connect(this, q, e);
}

void Context::icontext_accept(QueueProcessor<Event*>* q, AcceptEvent* e) {
    get_state()->state_accept(this, q, e);
}

void Context::icontext_new_connection_established(QueueProcessor<Event*>* q, ConnectionEstablishedEvent* e) {
    get_state()->state_new_connection_established(this, q, e);
}

void Context::icontext_new_connection_initiated(QueueProcessor<Event*>* q, ConnectionInitiatedEvent* e) {
    get_state()->state_new_connection_initiated(this, q, e);
}

void Context::icontext_close(QueueProcessor<Event*>* q, CloseEvent* e) {
    get_state()->state_close(this, q, e);
}

void Context::icontext_receive_packet(QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    get_state()->state_receive_packet(this, q, e);
}

void Context::icontext_send_packet(QueueProcessor<Event*>* q, SendPacketEvent* e) {
    get_state()->state_send_packet(this, q, e);
}

void Context::icontext_timer_fired_event(QueueProcessor<Event*>* q, TimerFiredEvent* e) {
    get_state()->state_timer_fired(this, q, e);
}

void Context::icontext_resend_packet(QueueProcessor<Event*>* q, ResendPacketEvent* e) {
    get_state()->state_resend_packet(this, q, e);
}

void Context::icontext_send(QueueProcessor<Event*>* q, SendEvent* e) {
    get_state()->state_send(this, q, e);
}

void Context::icontext_receive(QueueProcessor<Event*>* q, ReceiveEvent* e) {
    get_state()->state_receive(this, q, e);
}

void Context::icontext_receive_buffer_not_empty(QueueProcessor<Event*>* q, ReceiveBufferNotEmptyEvent* e) {
    get_state()->state_receive_buffer_not_empty(this, q, e);
}

void Context::icontext_receive_buffer_not_full(QueueProcessor<Event*>* q, ReceiveBufferNotFullEvent* e) {
    get_state()->state_receive_buffer_not_full(this, q, e);
}

void Context::icontext_send_buffer_not_empty(QueueProcessor<Event*>* q, SendBufferNotEmptyEvent* e) {
    get_state()->state_send_buffer_not_empty(this, q, e);
}

void Context::icontext_send_buffer_not_full(QueueProcessor<Event*>* q, SendBufferNotFullEvent* e) {
    get_state()->state_send_buffer_not_full(this, q, e);
}

void Context::icontext_delete_socket(QueueProcessor<Event*>* q, DeleteSocketEvent* e) {
    get_state()->state_delete_socket(this, q, e);
}

void Context::icontext_set_socket_option(QueueProcessor<Event*>* q, SetSocketOptionEvent* e) {
    get_state()->state_set_socket_option(this, q, e);
}

void Context::icontext_get_socket_option(QueueProcessor<Event*>* q, GetSocketOptionEvent* e) {
    get_state()->state_get_socket_option(this, q, e);
}

bool Context::icontext_can_send(Socket* s) {
    return get_state()->state_can_send(this, s);
}

bool Context::icontext_can_receive(Socket* s) {
    return get_state()->state_can_receive(this, s);
}
