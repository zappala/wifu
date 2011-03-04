#include "contexts/ConnectionManagerContext.h"

ConnectionManagerContext::ConnectionManagerContext() : Context() {
    set_state(new Closed());
}

void ConnectionManagerContext::socket(SocketEvent* e) {
    get_state()->socket(this, e);
}

void ConnectionManagerContext::bind(BindEvent* e) {
    get_state()->bind(this, e);
}

// TODO: move this method (and all others that we can) into Context


void ConnectionManagerContext::listen(ListenEvent* e) {
    get_state()->listen(this, e);
}

void ConnectionManagerContext::connect(ConnectEvent* e) {
    cout << "In ConnectionManagerContext::connect()" << endl;
    cout << "Current State: " << type_name(*get_state()) << endl;
    get_state()->connect(this, e);
}

void ConnectionManagerContext::accept(AcceptEvent* e) {
    get_state()->accept(this, e);
}

void ConnectionManagerContext::new_connection_established(ConnectionEstablishedEvent* e) {
    get_state()->new_connection_established(this, e);
}

void ConnectionManagerContext::close() {
    get_state()->close(this);
}

void ConnectionManagerContext::receive_packet(NetworkReceivePacketEvent* e) {
    get_state()->receive_packet(this, e);
}

void ConnectionManagerContext::send_packet(SendPacketEvent* e) {
    cout << "ConnectionManagerContext::send_packet()" << endl;
    //cout << "Current state: " << type_name(*get_state()) << endl;
    get_state()->send_packet(this, e);
}

void ConnectionManagerContext::timer_fired_event(TimerFiredEvent* e) {
    get_state()->timer_fired(this, e);
}

void ConnectionManagerContext::resend_packet(ResendPacketEvent* e) {
    cout << "ConnectionManagerContext::resend_packet()" << endl;
    get_state()->resend_packet(this, e);
}

// Non-state methods

int ConnectionManagerContext::get_back_log() {
    return back_log_;
}

void ConnectionManagerContext::set_back_log(int back_log) {
    back_log_ = back_log;
}

ConnectionType ConnectionManagerContext::get_connection_type() {
    return type_;
}

void ConnectionManagerContext::set_connection_type(ConnectionType type) {
    type_ = type;
}

ConnectEvent* ConnectionManagerContext::get_connect_event() {
    return c_event_;
}

void ConnectionManagerContext::set_connect_event(ConnectEvent* e) {
    c_event_ = e;
}

AcceptEvent* ConnectionManagerContext::get_accept_event() {
    return a_event_;
}

void ConnectionManagerContext::set_accept_event(AcceptEvent* e) {
    a_event_ = e;
}