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

class ReliabilityContext : public Context {
public:

    ReliabilityContext() : Context() {
        set_state(new ReliabilityState());
    }

    virtual ~ReliabilityContext() {

    }

    void connect(string& dest) {
        get_state()->connect(this, dest);
    }

    void close() {
        get_state()->close(this);
    }

    void receive(string& data) {
        get_state()->receive(this, data);
    }

    void send(string& dest, string& data) {
        get_state()->send(this, data);
    }

    bool is_open() {
        return get_state()->is_open();
    }
};

#endif	/* RELIABILITYCONTEXT_H */

