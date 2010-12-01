/* 
 * File:   Socket.h
 * Author: rbuck
 *
 * Created on October 30, 2010, 9:18 AM
 */

#ifndef _SOCKET_H
#define	_SOCKET_H

#include <string>

#include "Event.h"
#include "ConnectEvent.h"
#include "Dispatcher.h"
#include "Identifiable.h"
#include "PortManager.h"

using namespace std;

class Socket : public Identifiable {
public:

    /**
     * Creates a Socket object.
     */
    Socket(int protocol, AddressPort* local = new AddressPort("0.0.0.0", PortManager::instance().next())) : Identifiable(), protocol_(protocol), local_(local), remote_(0) {

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
    }

    /**
     * @return The int which represents this Socket.
     */
    int& get_socket() {
        return get_id();
    }

    int& get_protocol() {
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
        local_ = local;
    }

    void set_remote_address_port(AddressPort* remote) {
        remote_ = remote;
    }

private:
    AddressPort* local_;
    AddressPort* remote_;
    int protocol_;
};

#endif	/* _SOCKET_H */

