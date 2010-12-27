/* 
 * File:   Socket.h
 * Author: rbuck
 *
 * Created on October 30, 2010, 9:18 AM
 */

#ifndef _SOCKET_H
#define	_SOCKET_H

#include <string>

#include "events/Event.h"
#include "events/ConnectEvent.h"
#include "Dispatcher.h"
#include "PortManager.h"
#include "SocketManager.h"


using namespace std;

class Socket {
public:

    /**
     * Creates a Socket object.
     */
    Socket( int domain,
            int type,
            int protocol,
            AddressPort* local = new AddressPort("0.0.0.0", PortManager::instance().get()),
            AddressPort* remote = new AddressPort("0.0.0.0", PortManager::instance().get())) :
            socket_(SocketManager::instance().get()),
            domain_(domain),
            type_(type),
            protocol_(protocol),
            local_(local),
            remote_(remote) {

    }

    /**
     * Cleans up this Socket object.
     */
    virtual ~Socket() {
        if(local_) {
            delete local_;
        }

        if(remote_) {
            delete remote_;
        }

        SocketManager::instance().remove(socket_);
    }

    /**
     * @return The int which represents this Socket.
     */
    int get_socket() const {
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

    AddressPort* get_local_address_port() {
        assert(local_);
        return local_;
    }

    AddressPort* get_remote_address_port() {
        return remote_;
    }

    void set_local_address_port(AddressPort* local) {
        if(local_) {
            delete local_;
        }
        local_ = local;
    }

    void set_remote_address_port(AddressPort* remote) {
        if(remote_) {
            delete remote_;
        }
        remote_ = remote;
    }

    bool operator==(const Socket& other) {
        return get_socket() == other.get_socket();
    }

    bool operator!=(const Socket& other) {
        return !(operator ==(other));
    }




private:
    AddressPort* local_;
    AddressPort* remote_;
    int domain_;
    int type_;
    int protocol_;
    int socket_;
};

#endif	/* _SOCKET_H */

