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
#include "../AddressPort.h"
#include "../Socket.h"
#include "../Packet.h"
#include "../events/ConnectEvent.h"

using namespace std;

enum ConnectionType {
    PASSIVE_OPEN,
    ACTIVE_OPEN,
    ESTABLISHED
};

class ConnectionManagerContext : public Context {
public:
    ConnectionManagerContext();
    void listen(Socket* s, int back_log);
    void connect(ConnectEvent* e);
    void accept(Socket* s);
    void close();
    void receive(Socket* s, Packet* p);
    void send(Socket* s, Packet* p);
    bool is_open();

    // Non-state methods

    int get_back_log();
    void set_back_log(int back_log);

    ConnectEvent* get_connect_event();
    void set_connect_event(ConnectEvent* e);

    ConnectionType get_connection_type();
    void set_connection_type(ConnectionType type);

private:
    int back_log_;
    ConnectionType type_;
    ConnectEvent* c_event_;
};

#endif	/* CONNECTIONMANAGERCONTEXT_H */

