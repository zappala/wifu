/* 
 * File:   ConnectionManagerContext.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 10:36 AM
 */

#ifndef CONNECTIONMANAGERCONTEXT_H
#define	CONNECTIONMANAGERCONTEXT_H

#include <string>
#include "Context.h"
#include "states/Closed.h"
#include "AddressPort.h"
#include "Socket.h"
#include "packet/WiFuPacket.h"
#include "events/ConnectEvent.h"
#include "events/AcceptEvent.h"
#include "events/TimerFiredEvent.h"

using namespace std;

enum ConnectionType {
    PASSIVE_OPEN,
    ACTIVE_OPEN,
    ESTABLISHED
};

class ConnectionManagerContext : public Context {
public:
    ConnectionManagerContext();

    void socket(SocketEvent* e);
    void bind(BindEvent* e);
    void listen(ListenEvent* e);
    void connect(ConnectEvent* e);
    void accept(AcceptEvent* e);
    void new_connection_established(ConnectionEstablishedEvent* e);
    void close();
    void receive_packet(NetworkReceivePacketEvent* e);
    void send_packet(SendPacketEvent* e);
    void timer_fired_event(TimerFiredEvent* e);
    void resend_packet(ResendPacketEvent* e);

    // Non-state methods

    int get_back_log();
    void set_back_log(int back_log);

    ConnectEvent* get_connect_event();
    void set_connect_event(ConnectEvent* e);

    AcceptEvent* get_accept_event();
    void set_accept_event(AcceptEvent* e);

    ConnectionType get_connection_type();
    void set_connection_type(ConnectionType type);

private:
    int back_log_;
    ConnectionType type_;
    ConnectEvent* c_event_;
    AcceptEvent* a_event_;

};

#endif	/* CONNECTIONMANAGERCONTEXT_H */
