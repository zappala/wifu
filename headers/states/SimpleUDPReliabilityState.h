/* 
 * File:   SimpleUDPReliabilityState.h
 * Author: leer
 *
 * Created on June 21, 2011, 10:18 AM
 */

#ifndef _SIMPLEUDPRELIABILITYSTATE_H
#define	_SIMPLEUDPRELIABILITYSTATE_H

#include "State.h"
#include "events/protocol_events/SendPacketEvent.h"
#include "events/framework_events/NetworkReceivePacketEvent.h"
#include "events/framework_events/RecvFromResponseEvent.h"
#include "packet/UDPPacket.h"

#include "ObjectPool.h"

#include "contexts/SimpleUDPReliabilityContext.h"
#include "Math.h"

class SimpleUDPReliabilityState : public State {
public:
    SimpleUDPReliabilityState();
    virtual ~SimpleUDPReliabilityState();

    virtual void state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e);
    virtual void state_timer_fired(Context* c, QueueProcessor<Event*>* q, TimerFiredEvent* e);
    virtual void state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);
    virtual void state_receive_buffer_not_empty(Context* c, QueueProcessor<Event*>* q, ReceiveBufferNotEmptyEvent* e);
    virtual void state_receive(Context* c, QueueProcessor<Event*>* q, ReceiveEvent* e);
    virtual void state_send_buffer_not_empty(Context* c, QueueProcessor<Event*>* q, SendBufferNotEmptyEvent* e);

private:
    void create_and_dispatch_received_data(Context* c, QueueProcessor<Event*>* q, ReceiveEvent* e);

    void handle_data(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);
    void send_packets(Context* c, QueueProcessor<Event*>* q, Event* e);
    void send_one_packet(Context* c, QueueProcessor<Event*>* q, Event* e);
};

#endif	/* _SIMPLEUDPRELIABILITYSTATE_H */

