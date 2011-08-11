#include "contexts/ConnectionManagerContext.h"

ConnectionManagerContext::ConnectionManagerContext() : Context() {
    set_state(new Closed());
}

ConnectionManagerContext::~ConnectionManagerContext() {
    
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

sockaddr_un* ConnectionManagerContext::get_front_end_socket() {
    if(c_event_) {
        return c_event_->get_source();
    }
    return a_event_->get_source();
}

Socket* ConnectionManagerContext::get_socket() {
    if(c_event_) {
        return c_event_->get_socket();
    }
    return a_event_->get_socket();
}