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
#include "events/ConnectEvent.h"
#include "events/AcceptEvent.h"
#include "events/TimerFiredEvent.h"

using namespace std;

class State;

class Context : public IContext {
public:
    Context();
    virtual ~Context();
    virtual void set_state(State* s);
    inline State* get_state();

    // From IContext
    virtual void socket(SocketEvent* e);
    virtual void bind(BindEvent* e);
    virtual void listen(ListenEvent* e);
    virtual void connect(ConnectEvent* e);
    virtual void accept(AcceptEvent* e);
    virtual void new_connection_established(ConnectionEstablishedEvent* e);
    virtual void new_conneciton_initiated(ConnectionInitiatedEvent* e);
    virtual void close();
    virtual void receive_packet(NetworkReceivePacketEvent* e);
    virtual void send_packet(SendPacketEvent* e);
    virtual void timer_fired_event(TimerFiredEvent* e);
    virtual void resend_packet(ResendPacketEvent* e);

private:
    class State* current_;
};

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

#endif	/* CONTEXT_H */
