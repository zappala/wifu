/* 
 * File:   Protocol.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 10:34 AM
 */

#ifndef PROTOCOLCONTEXT_H
#define	PROTOCOLCONTEXT_H

#include <string>
#include <vector>

#include "IContext.h"
#include "Context.h"
#include "ContextContainer.h"
#include "../AddressPort.h"
#include "../Socket.h"
#include "../packet/WiFuPacket.h"
#include "../events/ConnectEvent.h"
#include "../events/AcceptEvent.h"

using namespace std;

class ProtocolContext : public IContext {
public:

    ProtocolContext() {

    }

    virtual ~ProtocolContext() {

    }

    void set_contexts(ContextContainer* c) {
        contexts_ = c->get_contexts();
    }

    virtual void add_context(Context* c) {
        contexts_.push_back(c);
    }

    void listen(Socket* s, int back_log) {
        for(int i = 0; i < contexts_.size(); ++i) {
            contexts_[i]->listen(s, back_log);
        }
    }

    // TODO: can we refactor out this for loop in each of these methods?
    // Maybe we can put it in the ContextContainter?
    virtual void send(Socket* s, WiFuPacket* p) {
        //cout << "Protocol: SEND" << endl;
        for(int i = 0; i < contexts_.size(); ++i) {
            contexts_[i]->send(s, p);
        }
        //cout << endl;
    }
    virtual void receive(Socket* s, WiFuPacket* p) {
        //cout << "Protocol: RECEIVE" << endl;
        for(int i = 0; i < contexts_.size(); ++i) {
            contexts_[i]->receive(s, p);
        }
        //cout << endl;
    }
    virtual void connect(ConnectEvent* e) {
        //cout << "Protocol: CONNECT" << endl;
        for(int i = 0; i < contexts_.size(); ++i) {
            contexts_[i]->connect(e);
        }
        //cout << endl;
    }

    virtual void accept(AcceptEvent* e) {
        //cout << "Protocol: ACCEPT" << endl;
        for(int i = 0; i < contexts_.size(); ++i) {
            contexts_[i]->accept(e);
        }
        //cout << endl;
    }

    virtual void connection_established(Socket* s) {
        //cout << "Protocol: CONNECTION ESTABLISHED" << endl;
        for(int i = 0; i < contexts_.size(); ++i) {
            contexts_[i]->connection_established(s);
        }
        //cout << endl;
    }

    virtual void close() {
        //cout << "Protocol: CLOSE" << endl;
        for(int i = 0; i < contexts_.size(); ++i) {
            contexts_[i]->close();
        }
        //cout << endl;
    }

private:
    vector<Context*> contexts_;
};

#endif	/* PROTOCOLCONTEXT_H */

