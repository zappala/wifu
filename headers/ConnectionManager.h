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
#include "SendSynEvent.h"

class ConnnectionManager : public Module {
public:

    ConnnectionManager() : Module() {

    }

    virtual ~ConnnectionManager() {
        cout << "";
    }

    void connect(Event* e) {
        ConnectEvent* c = (ConnectEvent*) e;
        cout << "Connection Manager Connect: " << c->get_address() << " " << c->get_port() << endl;

        Dispatcher::instance().enqueue(new SendSynEvent(c->get_socket(), c->get_address(), c->get_port()));
    }

    void data(Event* e) {

    }

};


#endif	/* _CONNECTIONMANAGER_H */

