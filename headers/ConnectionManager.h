/* 
 * File:   QConnectionManager.h
 * Author: rbuck
 *
 * Created on October 29, 2010, 2:12 PM
 */

#ifndef _CONNECTIONMANAGER_H
#define	_CONNECTIONMANAGER_H

#include "Module.h"
#include "SocketConnectionManager.h"
#include "ConnectEvent.h"

class ConnnectionManager : public Module {
public:

    ConnnectionManager() : Module() {
        s_ = "Connection Manager blah";
    }

    virtual ~ConnnectionManager() {

    }

    void process(Event* e) {
        e->execute(this);
    }

    void connect(Event* e) {
        ConnectEvent* c = (ConnectEvent*) e;
        cout << "Socket: " << c->get_socket() << endl;
        cout << get_string() << endl;
    }

    void data(Event* e) {

    }

    string & get_string() {
        return s_;
    }

private:
    string s_;
};


#endif	/* _CONNECTIONMANAGER_H */

