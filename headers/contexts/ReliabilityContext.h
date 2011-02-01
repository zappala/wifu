/* 
 * File:   ReliabilityContext.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 10:36 AM
 */

#ifndef RELIABILITYCONTEXT_H
#define	RELIABILITYCONTEXT_H

#include "Context.h"
#include "../states/ReliabilityState.h"
#include "../AddressPort.h"
#include "../Socket.h"
#include "../events/ConnectEvent.h"
#include "../events/AcceptEvent.h"
#include "../packet/WiFuPacket.h"

class ReliabilityContext : public Context {
public:

    ReliabilityContext();
    virtual ~ReliabilityContext();
    void listen(Socket* s, int back_log);
    void connect(ConnectEvent* e);
    void accept(AcceptEvent* e);
    void connection_established(Socket* s);
    void close();
    void receive(Socket* s, WiFuPacket* p);
    void send(Socket* s, WiFuPacket* p);
};

#endif	/* RELIABILITYCONTEXT_H */

