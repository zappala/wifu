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

    }

    virtual ~ConnnectionManager() {

    }

    void connect(Event* e) {
        ConnectEvent* c = (ConnectEvent*) e;
        cout << "Connection Manager Connect: " << c->get_address() << endl;
    }

    void data(Event* e) {

    }

};


#endif	/* _CONNECTIONMANAGER_H */

