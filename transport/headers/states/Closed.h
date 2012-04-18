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
#include "events/protocol_events/SendPacketEvent.h"
#include "events/framework_events/ConnectEvent.h"
#include "events/protocol_events/ConnectionEstablishedEvent.h"
#include "SourceGetter.h"

using namespace std;

class Closed : public State {
public:
    Closed();
    virtual ~Closed();
    virtual void state_connect(Context* c, QueueProcessor<Event*>* q, ConnectEvent* e);
    virtual void state_listen(Context* c, QueueProcessor<Event*>* q, ListenEvent* e);
    virtual void state_new_connection_established(Context* c, QueueProcessor<Event*>* q, ConnectionEstablishedEvent* e);
    virtual void state_new_connection_initiated(Context* c, QueueProcessor<Event*>* q, ConnectionInitiatedEvent* e);
    virtual void state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);

};

#endif	/* CLOSED_H */

