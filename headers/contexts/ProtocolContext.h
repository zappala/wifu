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
    virtual void send(string& destination, string& data) {
        //cout << "Protocol: SEND" << endl;
        for(int i = 0; i < contexts_.size(); ++i) {
            contexts_[i]->send(destination, data);
        }
        //cout << endl;
    }
    virtual void receive(string& data) {
        //cout << "Protocol: RECEIVE" << endl;
        for(int i = 0; i < contexts_.size(); ++i) {
            contexts_[i]->receive(data);
        }
        //cout << endl;
    }
    virtual void connect(string& destination) {
        //cout << "Protocol: CONNECT" << endl;
        for(int i = 0; i < contexts_.size(); ++i) {
            contexts_[i]->connect(destination);
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

    virtual bool is_open() {
        //cout << "Protocol: IS_OPEN" << endl;
        for(int i = 0; i < contexts_.size(); ++i) {
            if(contexts_[i]->is_open()) {
                return true;
            }
        }
        //cout << endl;
        return false;
    }

private:
    vector<Context*> contexts_;
};

#endif	/* PROTOCOLCONTEXT_H */

