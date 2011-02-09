/* 
 * File:   Socket.h
 * Author: rbuck
 *
 * Created on October 30, 2010, 9:18 AM
 */

#ifndef _SOCKET_H
#define	_SOCKET_H

#include <string>

#include "PortManager.h"
#include "SocketManager.h"
#include "GarbageCollector.h"
#include "AddressPort.h"
#include "observer/Observable.h"

using namespace std;
// TODO: do we need to protect this with a Semaphore, like a monitor?
// TODO: what about when we need to call notify()?  How should that be protected, if at all?

class Socket : public Observable, public gc {
public:

    /**
     * Creates a Socket object.
     */
    Socket(int domain,
            int type,
            int protocol,
            AddressPort* local = new AddressPort("0.0.0.0", PortManager::instance().get()),
            AddressPort* remote = new AddressPort("0.0.0.0", PortManager::instance().get())) :
    socket_(SocketManager::instance().get()),
    domain_(domain),
    type_(type),
    protocol_(protocol),
    local_(local),
    remote_(remote),
    is_passive_(false) {

        }

    /**
     * Cleans up this Socket object.
     */
    virtual ~Socket() {
        if (local_) {
            delete local_;
        }

        if (remote_) {
            delete remote_;
        }

        SocketManager::instance().remove(socket_);
    }

    /**
     * @return The int which represents this Socket.
     */
    int get_socket_id() const {
        return socket_;
    }

    int get_domain() const {
        return domain_;
    }

    int get_type() const {
        return type_;
    }

    int get_protocol() const {
        return protocol_;
    }

    void make_passive() {
        is_passive_ = true;
    }

    bool is_passive() const {
        return is_passive_;
    }

    AddressPort* get_local_address_port() {
        assert(local_);
        return local_;
    }

    AddressPort* get_remote_address_port() {
        return remote_;
    }

    void set_local_address_port(AddressPort* local) {
        if (local_) {
            delete local_;
        }
        local_ = local;
        notify();
    }

    void set_remote_address_port(AddressPort* remote) {
        if (remote_) {
            delete remote_;
        }
        remote_ = remote;
        notify();
    }

    bool operator==(const Socket& other) {
        return get_socket_id() == other.get_socket_id();
    }

    bool operator!=(const Socket& other) {
        return !(operator ==(other));
    }

    string get_key() {
        return make_key(get_local_address_port(), get_remote_address_port());
    }

    static string make_key(AddressPort* local, AddressPort* remote) {
        string local_s = local->to_s();
        string remote_s = remote->to_s();
        return local_s.append(remote_s);
    }




private:
    AddressPort* local_;
    AddressPort* remote_;
    int domain_;
    int type_;
    int protocol_;
    int socket_;

    bool is_passive_;
};

#endif	/* _SOCKET_H */

