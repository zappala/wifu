/* 
 * File:   Closed.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 1:05 PM
 */

#ifndef CLOSED_H
#define	CLOSED_H

#include <iostream>

#include "contexts/Context.h"
#include "contexts/ConnectionManagerContext.h"
#include "states/Listen.h"
#include "states/SynSent.h"
#include "AddressPort.h"
#include "packet/TCPPacket.h"
#include "events/SendPacketEvent.h"
#include "events/ConnectEvent.h"
#include "events/ConnectionEstablishedEvent.h"
#include "SourceGetter.h"

using namespace std;

class Closed : public State {
public:
    Closed();
    virtual ~Closed();
    virtual void state_connect(Context* c, ConnectEvent* e);
    virtual void state_listen(Context* c, ListenEvent* e);
    virtual void state_new_connection_established(Context* c, ConnectionEstablishedEvent* e);
    virtual void state_new_connection_initiated(Context* c, ConnectionInitiatedEvent* e);

};

#endif	/* CLOSED_H */

