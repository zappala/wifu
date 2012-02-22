/* 
 * File:   ConnectionEstablishedEvent.h
 * Author: rbuck
 *
 * Created on January 14, 2011, 3:50 PM
 */

#ifndef _CONNECTIONESTABLISHEDEVENT_H
#define	_CONNECTIONESTABLISHEDEVENT_H

#include "ProtocolEvent.h"
#include "events/framework_events/AcceptEvent.h"

using namespace std;

/**
 * ProtocolEvent which signals that a Connection has been established with a remote host.
 * This may only be created by a connection manager doing a passive open.
 *
 * @see Event
 * @see ProtocolEvent
 * @see AcceptEvent
 */
class ConnectionEstablishedEvent : public ProtocolEvent {
public:

    /**
     * Constructs a ConnectionEstablishedEvent.
     * @param listening_socket The socket used to receive the SYN.
     * @param new_socket The new socket to send and receive packets on. (Not the one that accept() is acting on.)
     */
    ConnectionEstablishedEvent(Socket* listening_socket, Socket* new_socket);

    /**
     * Destructor.
     */
    virtual ~ConnectionEstablishedEvent();

    /**
     * Calls IModule::imodule_connection_established() and passes this ConnectionEstablishedEvent in as the argument.
     *
     * @param m The module which to call IModule::imodule_connection_established() on.
     * @see Event::execute()
     * @see IModule
     * @see IModule::imodule_connection_established()
     */
    void execute(IModule* m);

    /**
     * @return The new socket to send and receive packets on.
     */
    Socket* get_new_socket();

private:

    /**
     * The new socket to send and receive packets on. (Not the one that accept() is acting on.)
     */
    Socket* new_socket_;
};

#endif	/* _CONNECTIONESTABLISHEDEVENT_H */
