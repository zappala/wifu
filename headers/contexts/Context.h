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
    State* get_state();

private:
    class State* current_;
};

class State {
public:
    State();

    virtual ~State();

    virtual void enter(Context*);

    virtual void exit(Context*);

    virtual void socket(Context*, Socket*);

    virtual void bind(Context*, Socket*, AddressPort*);

    virtual void listen(Context*, Socket*, int);

    virtual void accept(Context*, AcceptEvent*);

    virtual void new_connection_established(Context*, Socket*);

    virtual void receive_packet(Context*, Socket*, WiFuPacket*);

    virtual void timer_fired(Context*, TimerFiredEvent*);

    virtual void connect(Context*, ConnectEvent*);

    virtual void close(Context*);

    virtual void send_packet(Context*, Socket*, WiFuPacket*);

    void enter_state(string);

    void leave_state(string);

};

#endif	/* CONTEXT_H */
