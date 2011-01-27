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
#include "../packet/WiFuPacket.h"
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
    void receive(Socket* s, WiFuPacket* p);
    void send(Socket* s, WiFuPacket* p);
};

#endif	/* CONGESTIONCONTROLCONTEXT_H */

