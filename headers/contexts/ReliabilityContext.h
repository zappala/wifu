/* 
 * File:   ReliabilityContext.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 10:36 AM
 */

#ifndef RELIABILITYCONTEXT_H
#define	RELIABILITYCONTEXT_H

#include "Context.h"
#include "../states/ReliabilityState.h"
#include "../AddressPort.h"
#include "../Socket.h"
#include "../events/ConnectEvent.h"

class ReliabilityContext : public Context {
public:

    ReliabilityContext() : Context() {
        set_state(new ReliabilityState());
    }

    virtual ~ReliabilityContext() {

    }

    void listen(Socket* s, int back_log) {
        get_state()->listen(this, s, back_log);
    }

    void connect(ConnectEvent* e) {
        get_state()->connect(this, e);
    }

    void close() {
        get_state()->close(this);
    }

    void receive(Socket* s, Packet* p) {
        get_state()->receive(this, s, p);
    }

    void send(Socket* s, Packet* p) {
        get_state()->send(this, s, p);
    }

    bool is_open() {
        return get_state()->is_open();
    }
};

#endif	/* RELIABILITYCONTEXT_H */

