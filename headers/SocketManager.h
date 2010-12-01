/* 
 * File:   SocketManager.h
 * Author: rbuck
 *
 * Created on December 1, 2010, 2:33 PM
 */

#ifndef _SOCKETMANAGER_H
#define	_SOCKETMANAGER_H

#include "Semaphore.h"
#include "AddressPort.h"
#include "Socket.h"

class SocketManager {
public:

    static SocketManager& instance() {
        static SocketManager instance_;
        return instance_;
    }

    virtual ~SocketManager() {

    }

    Socket* get(const int key) {
        mutex_.wait();
        AddressPort* ptr = int_to_socket_map_[key];
        mutex_.post();
        return ptr;
    }

    Socket* get(const AddressPort* key) {
        mutex_.wait();
        AddressPort* ptr = address_port_to_socket_map_[key];
        mutex_.post();
        return ptr;
    }

    void put(int key, Socket* value) {
        mutex_.wait();
        int_to_socket_map_[key] = value;
        address_port_to_socket_map_[value->get_local_address_port()] = value;
        mutex_.post();
    }

    void put(AddressPort* key, Socket* value) {
        mutex_.wait();
        address_port_to_socket_map_[key] = value;
        int_to_socket_map_[value->get_socket()] = value;
        mutex_.post();
    }

    void erase_at(const int key) {
        mutex_.wait();
        Socket* value = int_to_socket_map_[key];

        if (value) {
            map<AddressPort*, Socket*>::iterator itr;
            AddressPort* ptr;
            for (itr = address_port_to_socket_map_.begin(); itr != address_port_to_socket_map_.end(); ++itr) {
                if (*(itr->first) == *(value->get_local_address_port())) {
                    ptr = itr->first;
                    break;
                }
            }
            address_port_to_socket_map_.erase(ptr);
            delete ptr;

            int_to_socket_map_.erase(key);
            delete value;
        }
        
        mutex_.post();
    }

private:

    SocketManager() {
        mutex_.init(1);
    }

    Semaphore mutex_;

    map<int, Socket*> int_to_socket_map_;
    map<AddressPort*, Socket*, AddressPortComparator> address_port_to_socket_map_;

};

#endif	/* _SOCKETMANAGER_H */

