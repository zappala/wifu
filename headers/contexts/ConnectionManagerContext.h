/* 
 * File:   ConnectionManagerContext.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 10:36 AM
 */

#ifndef CONNECTIONMANAGERCONTEXT_H
#define	CONNECTIONMANAGERCONTEXT_H

#include <string>
#include "Context.h"
#include "../states/Closed.h"

using namespace std;

class ConnectionManagerContext : public Context {
public:
    ConnectionManagerContext() : Context() {
        set_state(new Closed());
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

#endif	/* CONNECTIONMANAGERCONTEXT_H */

