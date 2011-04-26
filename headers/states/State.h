/*
 * State.h
 *
 *  Created on: Mar 5, 2011
 *      Author: erickson
 */

#ifndef STATE_H_
#define STATE_H_

#include "events/SocketEvent.h"
#include "events/BindEvent.h"
#include "events/ListenEvent.h"
#include "events/AcceptEvent.h"
#include "events/ConnectionEstablishedEvent.h"
#include "events/ConnectionInitiatedEvent.h"
#include "events/NetworkReceivePacketEvent.h"
#include "events/TimerFiredEvent.h"
#include "events/ConnectEvent.h"
#include "events/SendPacketEvent.h"
#include "events/ResendPacketEvent.h"
#include "events/CloseEvent.h"
#include "events/DeleteSocketEvent.h"
#include "events/ReceiveBufferNotFullEvent.h"
#include "events/SetSocketOptionEvent.h"
#include "events/GetSocketOptionEvent.h"

#include <string>
#include "contexts/Context.h"

using namespace std;

class Context;

class State {
public:
    State();

    virtual ~State();

    virtual void state_enter(Context*);

    virtual void state_exit(Context*);

    virtual void state_socket(Context*, SocketEvent* e);

    virtual void state_bind(Context*, BindEvent* e);

    virtual void state_listen(Context*, ListenEvent* e);

    virtual void state_accept(Context*, AcceptEvent*);

    virtual void state_send(Context*, SendEvent*);

    virtual bool state_can_send(Context*, Socket*);

    virtual bool state_can_receive(Context*, Socket*);

    virtual void state_receive(Context*, ReceiveEvent*);

    virtual void state_new_connection_established(Context*, ConnectionEstablishedEvent* e);

    virtual void state_new_connection_initiated(Context*, ConnectionInitiatedEvent* e);

    virtual void state_receive_packet(Context*, NetworkReceivePacketEvent* e);

    virtual void state_timer_fired(Context*, TimerFiredEvent*);

    virtual void state_connect(Context*, ConnectEvent*);

    virtual void state_close(Context*, CloseEvent*);

    virtual void state_send_packet(Context*, SendPacketEvent* e);

    virtual void state_resend_packet(Context*, ResendPacketEvent* e);

    virtual void state_send_buffer_not_empty(Context*, SendBufferNotEmptyEvent*);

    virtual void state_send_buffer_not_full(Context*, SendBufferNotFullEvent*);

    virtual void state_receive_buffer_not_empty(Context*, ReceiveBufferNotEmptyEvent*);

    virtual void state_receive_buffer_not_full(Context*, ReceiveBufferNotFullEvent*);

    virtual void state_delete_socket(Context*, DeleteSocketEvent*);

    virtual void state_set_socket_option(Context*, SetSocketOptionEvent*);

    virtual void state_get_socket_option(Context*, GetSocketOptionEvent*);

};

#endif /* STATE_H_ */
