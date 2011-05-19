#include "Socket.h"

Socket::Socket(int domain,
        int type,
        int protocol,
        AddressPort* local,
        AddressPort* remote) :
    Observable(),
    socket_(SocketManager::instance().get()),
    domain_(domain),
    type_(type),
    protocol_(protocol),
    local_(local),
    remote_(remote),
    is_passive_(false) {

}

Socket::~Socket() {
    SocketManager::instance().remove(socket_);
    PortManagerFactory::instance().create().remove(local_->get_port());
    PortManagerFactory::instance().create().remove(remote_->get_port());
}

int Socket::get_socket_id() const {
    return socket_;
}

int Socket::get_domain() const {
    return domain_;
}

int Socket::get_type() const {
    return type_;
}

int Socket::get_protocol() const {
    return protocol_;
}

void Socket::make_passive() {
    is_passive_ = true;
}

bool Socket::is_passive() const {
    return is_passive_;
}

AddressPort* Socket::get_local_address_port() {
    assert(local_);
    return local_;
}

AddressPort* Socket::get_remote_address_port() {
    return remote_;
}

void Socket::set_local_address_port(AddressPort* local) {
    if (local_) {
        delete local_;
    }
    local_ = local;
    notify();
}

void Socket::set_remote_address_port(AddressPort* remote) {
    if (remote_) {
        delete remote_;
    }
    remote_ = remote;
    notify();
}

bool Socket::operator==(const Socket& other) {
    return get_socket_id() == other.get_socket_id();
}

bool Socket::operator!=(const Socket& other) {
    return !(operator ==(other));
}

// TODO: can we lazily evaluate this?

string Socket::get_key() {
    return make_key(get_local_address_port(), get_remote_address_port());
}

string Socket::make_key(AddressPort* local, AddressPort* remote) {
    string local_s = local->to_s();
    string remote_s = remote->to_s();
    return local_s.append(remote_s);
}

string& Socket::get_receive_buffer() {
    return receive_buffer_;
}

string& Socket::get_send_buffer() {
    return send_buffer_;
}

string& Socket::get_resend_buffer() {
    return resend_buffer_;
}

SocketOptions& Socket::get_socket_options() {
    return socket_options_;
}