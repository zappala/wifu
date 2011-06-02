/* 
 * File:   Closing.h
 * Author: rbuck
 *
 * Created on April 18, 2011, 4:36 PM
 */

#ifndef CLOSING_H
#define	CLOSING_H

#include "State.h"
#include "contexts/Context.h"
#include "events/framework_events/NetworkReceivePacketEvent.h"
#include "packet/TCPPacket.h"
#include "Socket.h"
#include "contexts/ConnectionManagerContext.h"
#include "states/TimeWait.h"

class Closing : public State {
public:
    Closing();
    virtual ~Closing();
    virtual void state_enter(Context* c);
    virtual void state_exit(Context* c);

    virtual void state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);
};

#endif	/* CLOSING_H */

