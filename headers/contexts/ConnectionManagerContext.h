/* 
 * File:   ConnectionManagerContext.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 10:36 AM
 */

#ifndef CONNECTIONMANAGERCONTEXT_H
#define	CONNECTIONMANAGERCONTEXT_H

#include "defines.h"
#include "Context.h"
#include "states/Closed.h"
#include "AddressPort.h"
#include "Socket.h"
#include "packet/WiFuPacket.h"
#include "events/framework_events/ConnectEvent.h"
#include "events/framework_events/AcceptEvent.h"
#include "events/framework_events/TimerFiredEvent.h"
#include "events/Event.h"
#include "events/framework_events/NetworkReceivePacketEvent.h"

using namespace std;

/**
 * Represents the type of connection we are conducting or established if we are already established.
 */
enum ConnectionType {
    PASSIVE_OPEN,
    ACTIVE_OPEN,
    ESTABLISHED
};

/**
 * Holds all the data for the connection manager FSM that has Closed as its entry point.
 * 
 * @see CloseWait
 * @see Closed
 * @see Closing
 * @see Established
 * @see FinWait1
 * @see FinWait2
 * @see LastAck
 * @see Listen
 * @see Open
 * @see SynReceived
 * @see SynSent
 * @see TimeWait
 */
class ConnectionManagerContext : public Context {
public:

    /**
     * Constructor.
     * Sets the initial state to Closed.
     * @see Closed.
     */
    ConnectionManagerContext();

    /**
     * Destructor.
     */
    virtual ~ConnectionManagerContext();

    /**
     * @return The ConnectEvent used to create a new connection.
     * @see ConnectEvent
     * @see WiFuTransportAPILocalSocket::wifu_connect()
     */
    ConnectEvent* get_connect_event();

    /**
     * Sets the ConnectEvent used to create a new connection.
     * @param e The ConnectEvent to save.
     * @see ConnectEvent
     * @see WiFuTransportAPILocalSocket::wifu_connect()
     */
    void set_connect_event(ConnectEvent* e);

    /**
     * @return The ListenEvent used to accept a new connection.
     * @see ListenEvent
     */
    ListenEvent* get_listen_event();

    /**
     * Sets the ListenEvent used to accept a new connection.
     * @param e The ListenEvent to save.
     * @see ListenEvent
     */
    void set_listen_event(ListenEvent* e);

    /**
     * @return The type of connection.
     * A connection can either be active, passive, or established.
     * @see ConnectionType
     */
    ConnectionType get_connection_type();

    /**
     * Sets the type of connetion that this FSM is performing.
     * @param type The type of connection this FSM is performing.  It can either be active, passive, or established.
     */
    void set_connection_type(ConnectionType type);

    /**
     * @return The unix socket that the front end is receiving messages on.
     */
    sockaddr_un* get_front_end_socket();

    /**
     * @return A pointer to the Socket which is using this conneciton manager FSM.
     */
    Socket* get_socket();

    void set_socket(Socket* s);

    void set_accept_socket(Socket* s);

    Socket* get_accept_socket();

private:

    /**
     * The type of connection this FSM is performing or performed.
     */
    ConnectionType type_;

    /**
     * The ConnectEvent received if this FSM peformed an active open.
     */
    ConnectEvent* c_event_;

    /**
     * The ListenEvent if this FSM performed a passive open.
     */
    ListenEvent* l_event_;

    Socket* socket_;

    Socket* accept_socket_;

};

#endif	/* CONNECTIONMANAGERCONTEXT_H */
