/* 
 * File:   LastAck.h
 * Author: rbuck
 *
 * Created on April 18, 2011, 4:43 PM
 */

#ifndef LASTACK_H
#define	LASTACK_H

#include "State.h"
#include "contexts/Context.h"
#include "events/framework_events/NetworkReceivePacketEvent.h"
#include "packet/TCPPacket.h"
#include "Socket.h"
#include "contexts/ConnectionManagerContext.h"
#include "events/protocol_events/DeleteSocketEvent.h"

class LastAck : public State {
public:
    LastAck();
    virtual ~LastAck();
    virtual void state_enter(Context* c);
    virtual void state_exit(Context* c);

    virtual void state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);
};


#endif	/* LASTACK_H */

