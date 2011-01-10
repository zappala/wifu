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

using namespace std;

class ConnectionManagerContext : public Context {
public:
    ConnectionManagerContext();
    void listen(Socket* s, int back_log);
    void connect(Socket* s, AddressPort& dest);
    void close();
    void receive(Socket* s, Packet* p);
    void send(Socket* s, Packet* p);
    bool is_open();

    // Non-state methods

    int get_back_log();
    void set_back_log(int back_log);

private:
    int back_log_;
};

#endif	/* CONNECTIONMANAGERCONTEXT_H */

