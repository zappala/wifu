/* 
 * File:   ConnectEvent.h
 * Author: rbuck
 *
 * Created on October 29, 2010, 2:27 PM
 */

#ifndef _CONNECTEVENT_H
#define	_CONNECTEVENT_H

#include "LibraryEvent.h"
#include "Socket.h"

/**
 * Event which represents a connect event.
 * @see Event
 */
class ConnectEvent : public LibraryEvent {
public:

    /**
     * Constructs a ConnectEvent.
     *
     */
    ConnectEvent(string& message, string& file, Socket* s) : LibraryEvent(message, file, s), destination_(0) {

    }

    virtual ~ConnectEvent() {

    }

    /**
     * Will call connect() on m.
     *
     * @param m The IModule which to call connect() on.
     * @see IModule::connect()
     */
    void execute(IModule* m) {
        m->library_connect(this);
    }

    AddressPort* get_destination() {
        if (!destination_) {
            string address = get_map()[ADDRESS_STRING];
            int port = atoi(get_map()[PORT_STRING].c_str());
            destination_ = new AddressPort(address, port);
        }
        return destination_;
    }

    void set_socket_destination() {
        get_socket()->set_remote_address_port(get_destination());
    }

private:
    AddressPort* destination_;
};

#endif	/* _CONNECTEVENT_H */

