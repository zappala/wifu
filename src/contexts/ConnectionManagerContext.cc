#include "contexts/ConnectionManagerContext.h"

ConnectionManagerContext::ConnectionManagerContext() : Context() {
    set_state(new Closed());
}

void ConnectionManagerContext::socket(Socket* s) {
    get_state()->socket(this, s);
}

void ConnectionManagerContext::bind(Socket* s, AddressPort* ap) {
    get_state()->bind(this, s, ap);
}

// TODO: move this method (and all others that we can) into Context


void ConnectionManagerContext::listen(Socket* s, int back_log) {
    get_state()->listen(this, s, back_log);
}

void ConnectionManagerContext::connect(ConnectEvent* e) {
    cout << "In ConnectionManagerContext::connect()" << endl;
    cout << "Current State: " << type_name(*get_state()) << endl;
    get_state()->connect(this, e);
}

void ConnectionManagerContext::accept(AcceptEvent* e) {
    get_state()->accept(this, e);
}

void ConnectionManagerContext::new_connection_established(Socket* s) {
    get_state()->new_connection_established(this, s);
}

void ConnectionManagerContext::close() {
    get_state()->close(this);
}

void ConnectionManagerContext::receive_packet(Socket* s, WiFuPacket* p) {
    get_state()->receive_packet(this, s, p);
}

void ConnectionManagerContext::send_packet(Socket* s, WiFuPacket* p) {
    cout << "ConnectionManagerContext::send_packet()" << endl;
    //cout << "Current state: " << type_name(*get_state()) << endl;
    get_state()->send_packet(this, s, p);
}

void ConnectionManagerContext::timer_fired_event(TimerFiredEvent* e) {
    get_state()->timer_fired(this, e);
}

void ConnectionManagerContext::resend_packet(Socket* s, WiFuPacket* p) {
    cout << "ConnectionManagerContext::resend_packet()" << endl;
    get_state()->resend_packet(this, s, p);
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