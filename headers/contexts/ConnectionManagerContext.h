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
 * @see Accept
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
     * @return The back log value as passed in via the listen() call.
     * @see WifuEndAPILocalSocket::wifu_listen()
     */
    int get_back_log();

    /**
     * Sets the back log value as passed in via the listen() call.
     * @see WifuEndAPILocalSocket::wifu_listen()
     */
    void set_back_log(int back_log);

    /**
     * @return The ConnectEvent used to create a new connection.
     * @see ConnectEvent
     * @see WifuEndAPILocalSocket::wifu_connect()
     */
    ConnectEvent* get_connect_event();

    /**
     * Sets the ConnectEvent used to create a new connection.
     * @param e The ConnectEvent to save.
     * @see ConnectEvent
     * @see WifuEndAPILocalSocket::wifu_connect()
     */
    void set_connect_event(ConnectEvent* e);

    /**
     * @return The AcceptEvent used to accept a new connection.
     * @see AcceptEvent
     * @see WifuEndAPILocalSocket::wifu_accept()
     */
    AcceptEvent* get_accept_event();

    /**
     * Sets the AcceptEvent used to accept a new connection.
     * @param e The AcceptEvent to save.
     * @see AcceptEvent
     * @see WifuEndAPILocalSocket::wifu_accept()
     */
    void set_accept_event(AcceptEvent* e);

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
     * @return The unix socket file that the front end is receiving messages on.
     */
    gcstring& get_file();

    /**
     * @return A pointer to the Socket which is using this conneciton manager FSM.
     */
    Socket* get_socket();

private:

    /**
     * The number of connections that this connection manager is willing to keep back logged.
     */
    int back_log_;

    /**
     * The type of connection this FSM is performing or performed.
     */
    ConnectionType type_;

    /**
     * The ConnectEvent received if this FSM peformed an active open.
     */
    ConnectEvent* c_event_;

    /**
     * The AcceptEvent if this FSM performed a passive open.
     */
    AcceptEvent* a_event_;

};

#endif	/* CONNECTIONMANAGERCONTEXT_H */
