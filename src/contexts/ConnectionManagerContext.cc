#include "contexts/ConnectionManagerContext.h"

ConnectionManagerContext::ConnectionManagerContext() : Context() {
    set_state(new Closed());
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

string& ConnectionManagerContext::get_file() {
    if(c_event_) {
        return c_event_->get_map()[FILE_STRING];
    }
    return a_event_->get_map()[FILE_STRING];
}

Socket* ConnectionManagerContext::get_socket() {
    if(c_event_) {
        return c_event_->get_socket();
    }
    return a_event_->get_socket();
}