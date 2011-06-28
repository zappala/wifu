/*
 * State.h
 *
 *  Created on: Mar 5, 2011
 *      Author: erickson
 */

#ifndef STATE_H_
#define STATE_H_

#include "events/framework_events/SocketEvent.h"
#include "events/framework_events/BindEvent.h"
#include "events/framework_events/ListenEvent.h"
#include "events/framework_events/AcceptEvent.h"
#include "events/protocol_events/ConnectionEstablishedEvent.h"
#include "events/protocol_events/ConnectionInitiatedEvent.h"
#include "events/framework_events/NetworkReceivePacketEvent.h"
#include "events/framework_events/TimerFiredEvent.h"
#include "events/framework_events/ConnectEvent.h"
#include "events/protocol_events/SendPacketEvent.h"
#include "events/protocol_events/ResendPacketEvent.h"
#include "events/framework_events/CloseEvent.h"
#include "events/protocol_events/DeleteSocketEvent.h"
#include "events/protocol_events/ReceiveBufferNotFullEvent.h"
#include "events/framework_events/SetSocketOptionEvent.h"
#include "events/framework_events/GetSocketOptionEvent.h"

#include "GarbageCollector.h"

#include <string>
#include "contexts/Context.h"

using namespace std;

class Context;

class State : public gc{
public:
    State();

    virtual ~State();

    virtual void state_enter(Context*);

    virtual void state_exit(Context*);

    virtual void state_socket(Context*, QueueProcessor<Event*>* q, SocketEvent* e);

    virtual void state_bind(Context*, QueueProcessor<Event*>* q, BindEvent* e);

    virtual void state_listen(Context*, QueueProcessor<Event*>* q, ListenEvent* e);

    virtual void state_accept(Context*, QueueProcessor<Event*>* q, AcceptEvent*);

    virtual void state_send(Context*, QueueProcessor<Event*>* q, SendEvent*);

    virtual bool state_can_send(Context*, Socket*);

    virtual bool state_can_receive(Context*, Socket*);

    virtual void state_receive(Context*, QueueProcessor<Event*>* q, ReceiveEvent*);

    virtual void state_new_connection_established(Context*, QueueProcessor<Event*>* q, ConnectionEstablishedEvent* e);

    virtual void state_new_connection_initiated(Context*, QueueProcessor<Event*>* q, ConnectionInitiatedEvent* e);

    virtual void state_receive_packet(Context*, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);

    virtual void state_timer_fired(Context*, QueueProcessor<Event*>* q, TimerFiredEvent*);

    virtual void state_connect(Context*, QueueProcessor<Event*>* q, ConnectEvent*);

    virtual void state_close(Context*, QueueProcessor<Event*>* q, CloseEvent*);

    virtual void state_send_packet(Context*, QueueProcessor<Event*>* q, SendPacketEvent* e);

    virtual void state_resend_packet(Context*, QueueProcessor<Event*>* q, ResendPacketEvent* e);

    virtual void state_send_buffer_not_empty(Context*, QueueProcessor<Event*>* q, SendBufferNotEmptyEvent*);

    virtual void state_send_buffer_not_full(Context*, QueueProcessor<Event*>* q, SendBufferNotFullEvent*);

    virtual void state_receive_buffer_not_empty(Context*, QueueProcessor<Event*>* q, ReceiveBufferNotEmptyEvent*);

    virtual void state_receive_buffer_not_full(Context*, QueueProcessor<Event*>* q, ReceiveBufferNotFullEvent*);

    virtual void state_delete_socket(Context*, QueueProcessor<Event*>* q, DeleteSocketEvent*);

    virtual void state_set_socket_option(Context*, QueueProcessor<Event*>* q, SetSocketOptionEvent*);

    virtual void state_get_socket_option(Context*, QueueProcessor<Event*>* q, GetSocketOptionEvent*);
};

#endif /* STATE_H_ */
