/* 
 * File:   ConnectEvent.h
 * Author: rbuck
 *
 * Created on October 29, 2010, 2:27 PM
 */

#ifndef _CONNECTEVENT_H
#define	_CONNECTEVENT_H

#include "Event.h"
#include "AddressPort.h"

/**
 * Event which represents a connect event.
 * @see Event
 */
class ConnectEvent : public Event, public AddressPort {

public:

    /**
     * Constructs a ConnectEvent.
     *
     * @param socket The socket, which represents a unique connection, to use for this Event
     * @param address The IP address of the machine to connect to.
     * @param port The port on machine represented by address which is listening for a connection.
     */
    ConnectEvent(int socket, string & address, string & port)
        : Event(socket), AddressPort(address, port) {

    }

    /**
     * Will call connect() on m.
     *
     * @param m The IModule which to call connect() on.
     * @see IModule::connect()
     */
    void execute(IModule* m) {
        m->connect(this);
    }
};

#endif	/* _CONNECTEVENT_H */

