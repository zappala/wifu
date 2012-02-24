#include "contexts/ConnectionManagerContext.h"

ConnectionManagerContext::ConnectionManagerContext() : Context(), l_event_(0), accept_socket_(0) {
    set_state(new Closed());
}

ConnectionManagerContext::~ConnectionManagerContext() {
    
}

// Non-state methods


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

ListenEvent* ConnectionManagerContext::get_listen_event() {
    return l_event_;
}

void ConnectionManagerContext::set_listen_event(ListenEvent* e) {
    l_event_ = e;
}

sockaddr_un* ConnectionManagerContext::get_front_end_socket() {
    if(c_event_) {
        return c_event_->get_source();
    }
    return l_event_->get_source();
}

Socket* ConnectionManagerContext::get_socket() {
    return socket_;
}

void ConnectionManagerContext::set_socket(Socket* s) {
    socket_ = s;
}

void ConnectionManagerContext::set_accept_socket(Socket* s) {
    accept_socket_ = s;
}

Socket* ConnectionManagerContext::get_accept_socket() {
    return accept_socket_;
}