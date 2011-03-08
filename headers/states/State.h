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

#include <string>
#include "contexts/Context.h"

using namespace std;

class Context;

class State {
public:
    State();

    virtual ~State();

    virtual void enter(Context*);

    virtual void exit(Context*);

    virtual void socket(Context*, SocketEvent* e);

    virtual void bind(Context*, BindEvent* e);

    virtual void listen(Context*, ListenEvent* e);

    virtual void accept(Context*, AcceptEvent*);

    virtual ssize_t send_to(Context*, SendEvent*);

    virtual bool is_connected(Context*, Socket*);

    virtual void receive_from(Context*, ReceiveEvent*);

    virtual void new_connection_established(Context*, ConnectionEstablishedEvent* e);

    virtual void new_connection_initiated(Context*, ConnectionInitiatedEvent* e);

    virtual void receive_packet(Context*, NetworkReceivePacketEvent* e);

    virtual void timer_fired(Context*, TimerFiredEvent*);

    virtual void connect(Context*, ConnectEvent*);

    virtual void close(Context*);

    virtual void send_packet(Context*, SendPacketEvent* e);

    virtual void resend_packet(Context*, ResendPacketEvent* e);

    void enter_state(string);

    void leave_state(string);

};

#endif /* STATE_H_ */
