/* 
 * File:   CongestionControlContext.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 10:37 AM
 */

#ifndef CONGESTIONCONTROLCONTEXT_H
#define	CONGESTIONCONTROLCONTEXT_H

#include <string>
#include "Context.h"
#include "../states/SlowStart.h"

using namespace std;

class CongestionControlContext : public Context {
public:

    CongestionControlContext() : Context() {
        set_state(new SlowStart());
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

#endif	/* CONGESTIONCONTROLCONTEXT_H */

