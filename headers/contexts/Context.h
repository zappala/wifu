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
    virtual void icontext_socket(SocketEvent* e);

    virtual void icontext_bind(BindEvent* e);

    virtual void icontext_listen(ListenEvent* e);

    virtual void icontext_connect(ConnectEvent* e);

    virtual void icontext_accept(AcceptEvent* e);

    virtual void icontext_new_connection_established(ConnectionEstablishedEvent* e);

    virtual void icontext_new_connection_initiated(ConnectionInitiatedEvent* e);

    virtual void icontext_close(CloseEvent* e);

    virtual void icontext_receive_packet(NetworkReceivePacketEvent* e);

    virtual void icontext_send_packet(SendPacketEvent* e);

    virtual void icontext_timer_fired_event(TimerFiredEvent* e);

    virtual void icontext_resend_packet(ResendPacketEvent* e);

    virtual void icontext_send(SendEvent* e);

    virtual bool icontext_can_send(Socket*);

    virtual bool icontext_can_receive(Socket*);

    virtual void icontext_receive(ReceiveEvent* e);

    virtual void icontext_send_buffer_not_empty(SendBufferNotEmptyEvent*);

    virtual void icontext_send_buffer_not_full(SendBufferNotFullEvent*);

    virtual void icontext_receive_buffer_not_empty(ReceiveBufferNotEmptyEvent*);

    virtual void icontext_receive_buffer_not_full(ReceiveBufferNotFullEvent*);

    virtual void icontext_delete_socket(DeleteSocketEvent*);

    virtual void icontext_set_socket_option(SetSocketOptionEvent*);

    virtual void icontext_get_socket_option(GetSocketOptionEvent*);

private:
    State* current_;

};

#endif	/* CONTEXT_H */
