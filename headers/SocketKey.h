/* 
 * File:   SocketKey.h
 * Author: rbuck
 *
 * Created on December 20, 2010, 5:25 PM
 */

#ifndef _SOCKETKEY_H
#define	_SOCKETKEY_H

#include <string>
#include <sstream>

#include "headers/AddressPort.h"

using namespace std;

class SocketKey {
public:

    SocketKey(int id) : id_(id), local_(0), remote_(0) {
    }

    SocketKey(AddressPort& local) : id_(0), remote_(0) {
        local_ = new AddressPort(local);
    }

    SocketKey(AddressPort& local, AddressPort& remote) : id_(0) {
        local_ = new AddressPort(local);
        remote_ = new AddressPort(remote);
    }

    ~SocketKey() {
        if (local_) {
            delete local_;
        }

        if (remote_) {
            delete remote_;
        }
    }

    static string get_key(int id, AddressPort& local, AddressPort& remote) {
        stringstream s;
        s << id << local.to_s() << remote.to_s();
        return s.str();
    }

private:
    int id_;
    AddressPort* local_;
    AddressPort* remote_;
};

#endif	/* _SOCKETKEY_H */

