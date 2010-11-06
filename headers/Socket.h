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

using namespace std;

/**
 * This class is meant to be used in place of the Unix socket and subsequent calls like listen, bind, connect, etc.
 */
class Socket : public Identifiable {
public:

    /**
     * Creates a Socket object.
     */
    Socket() {

    }

    /**
     * Cleans up this Socket object.
     */
    virtual ~Socket() {

    }

    /**
     * Connect to a machine represented by addresd and port.
     *
     * @param address The address to connect to.
     * @param port The port to connect to.
     */
    void connect(string & address, string & port) {
        Event* connect_event = new ConnectEvent(get_socket(), address, port);
        dispatch(connect_event);
    }

    /**
     * Sends message to machine represented by this Socket.
     *
     * @param message The message to send.
     */
    void send(string & message) {

    }

    /**
     * @return The int which represents this Socket.
     */
    int & get_socket() {
        return get_id();
    }

private:

    /**
     * Simply calls enqueue on the Dispatcher.
     *
     * @param e The Event object to enqueue on the Dispatcher.
     * @see Dispatcher::enqueue()
     */
    void dispatch(Event * e) {
        Dispatcher::instance().enqueue(e);
    }
};

#endif	/* _SOCKET_H */

