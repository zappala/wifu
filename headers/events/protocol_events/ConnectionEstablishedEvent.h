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
     * @param e The AcceptEvent that accepted a new connection.
     * @param new_socket The new socket to send and receive packets on. (Not the one that accept() is acting on.)
     */
    ConnectionEstablishedEvent(AcceptEvent* e, Socket* new_socket);

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
     * @return The new socket to send and receive packets on. (Not the one that accept() is acting on.)
     */
    Socket* get_new_socket();

    /**
     * @return The AcceptEvent that accepted a new connection.
     */
    AcceptEvent* get_accept_event();

private:

    /**
     * The new socket to send and receive packets on. (Not the one that accept() is acting on.)
     */
    Socket* new_socket_;

    /**
     * The AcceptEvent that accepted a new connection.
     */
    AcceptEvent* event_;

};

#endif	/* _CONNECTIONESTABLISHEDEVENT_H */
