/* 
 * File:   Protocol.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 10:34 AM
 */

#ifndef PROTOCOL_H
#define	PROTOCOL_H

#include <string>
#include <vector>

#include "Context.h"

using namespace std;

class Protocol : public IContext {
public:

    Protocol() {

    }

    virtual ~Protocol() {

    }

    vector<Context*>& get_contexts() {
        return contexts_;
    }

    virtual void send(string& destination, string& data) {
        cout << "Protocol: SEND" << endl;
        for(int i = 0; i < contexts_.size(); ++i) {
            contexts_[i]->send(destination, data);
        }
        cout << endl;
    }
    virtual void receive(string& data) {
        cout << "Protocol: RECEIVE" << endl;
        for(int i = 0; i < contexts_.size(); ++i) {
            contexts_[i]->receive(data);
        }
        cout << endl;
    }
    virtual void connect(string& destination) {
        cout << "Protocol: CONNECT" << endl;
        for(int i = 0; i < contexts_.size(); ++i) {
            contexts_[i]->connect(destination);
        }
        cout << endl;
    }

    virtual void close() {
        cout << "Protocol: CLOSE" << endl;
        for(int i = 0; i < contexts_.size(); ++i) {
            contexts_[i]->close();
        }
        cout << endl;
    }

    virtual bool is_open() {
        cout << "Protocol: IS_OPEN" << endl;
        for(int i = 0; i < contexts_.size(); ++i) {
            if(contexts_[i]->is_open()) {
                return true;
            }
        }
        cout << endl;
        return false;
    }

private:
    vector<Context*> contexts_;
};

#endif	/* PROTOCOL_H */

