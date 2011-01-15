/* 
 * File:   CongestionControlContext.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 10:37 AM
 */

#ifndef CONGESTIONCONTROLCONTEXT_H
#define	CONGESTIONCONTROLCONTEXT_H

#include <string>
#include "Context.h"
#include "../states/SlowStart.h"
#include "../AddressPort.h"
#include "../Socket.h"
#include "../Packet.h"
#include "../events/ConnectEvent.h"
#include "../events/AcceptEvent.h"

using namespace std;

class CongestionControlContext : public Context {
public:

    CongestionControlContext();
    void listen(Socket* s, int back_log);
    void connect(ConnectEvent* e);
    void accept(AcceptEvent* e);
    void connection_established(Socket* s);
    void close();
    void receive(Socket* s, Packet* p);
    void send(Socket* s, Packet* p);
    bool is_open();
};

#endif	/* CONGESTIONCONTROLCONTEXT_H */

