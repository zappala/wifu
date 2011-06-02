/* 
 * File:   Context.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 10:36 AM
 */

#ifndef CONTEXT_H
#define	CONTEXT_H

#include <string>
#include <iostream>
#include "IContext.h"

#include "AddressPort.h"
#include "Socket.h"
#include "events/framework_events/ConnectEvent.h"
#include "events/framework_events/AcceptEvent.h"
#include "events/framework_events/TimerFiredEvent.h"
#include "events/protocol_events/ReceiveBufferNotFullEvent.h"
#include "states/State.h"

using namespace std;

class State;

class Context : public IContext {
public:
    Context();

    virtual ~Context();

    virtual void set_state(State* s);

    virtual State* get_state();

    virtual string get_state_name();

    // From IContext
    virtual void icontext_socket(QueueProcessor<Event*>* q, SocketEvent* e);

    virtual void icontext_bind(QueueProcessor<Event*>* q, BindEvent* e);

    virtual void icontext_listen(QueueProcessor<Event*>* q, ListenEvent* e);

    virtual void icontext_connect(QueueProcessor<Event*>* q, ConnectEvent* e);

    virtual void icontext_accept(QueueProcessor<Event*>* q, AcceptEvent* e);

    virtual void icontext_new_connection_established(QueueProcessor<Event*>* q, ConnectionEstablishedEvent* e);

    virtual void icontext_new_connection_initiated(QueueProcessor<Event*>* q, ConnectionInitiatedEvent* e);

    virtual void icontext_close(QueueProcessor<Event*>* q, CloseEvent* e);

    virtual void icontext_receive_packet(QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);

    virtual void icontext_send_packet(QueueProcessor<Event*>* q, SendPacketEvent* e);

    virtual void icontext_timer_fired_event(QueueProcessor<Event*>* q, TimerFiredEvent* e);

    virtual void icontext_resend_packet(QueueProcessor<Event*>* q, ResendPacketEvent* e);

    virtual void icontext_send(QueueProcessor<Event*>* q, SendEvent* e);

    virtual bool icontext_can_send(Socket*);

    virtual bool icontext_can_receive(Socket*);

    virtual void icontext_receive(QueueProcessor<Event*>* q, ReceiveEvent* e);

    virtual void icontext_send_buffer_not_empty(QueueProcessor<Event*>* q, SendBufferNotEmptyEvent*);

    virtual void icontext_send_buffer_not_full(QueueProcessor<Event*>* q, SendBufferNotFullEvent*);

    virtual void icontext_receive_buffer_not_empty(QueueProcessor<Event*>* q, ReceiveBufferNotEmptyEvent*);

    virtual void icontext_receive_buffer_not_full(QueueProcessor<Event*>* q, ReceiveBufferNotFullEvent*);

    virtual void icontext_delete_socket(QueueProcessor<Event*>* q, DeleteSocketEvent*);

    virtual void icontext_set_socket_option(QueueProcessor<Event*>* q, SetSocketOptionEvent*);

    virtual void icontext_get_socket_option(QueueProcessor<Event*>* q, GetSocketOptionEvent*);

private:
    State* current_;

};

#endif	/* CONTEXT_H */
