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
            AddressPort* remote = new AddressPort("0.0.0.0", PortManager::instance().get()));

    /**
     * Cleans up this Socket object.
     */
    virtual ~Socket();

    /**
     * @return The int which represents this Socket.
     */
    int get_socket_id() const;

    int get_domain() const;

    int get_type() const;

    int get_protocol() const;

    void make_passive();

    bool is_passive() const;

    AddressPort* get_local_address_port();

    AddressPort* get_remote_address_port();

    void set_local_address_port(AddressPort* local);

    void set_remote_address_port(AddressPort* remote);

    bool operator==(const Socket& other);

    bool operator!=(const Socket& other);

    string get_key();

    static string make_key(AddressPort* local, AddressPort* remote);

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

