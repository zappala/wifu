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
    ConnectEvent(string& message, string& file, Socket* s) : LibraryEvent(message, file, s) {

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
        string address = get_map()[ADDRESS_STRING];
        int port = atoi(get_map()[PORT_STRING].c_str());
        return new AddressPort(address, port);
    }
};

#endif	/* _CONNECTEVENT_H */

