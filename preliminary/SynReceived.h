/* 
 * File:   SynReceived.h
 * Author: rbuck
 *
 * Created on December 17, 2010, 3:16 PM
 */

#ifndef _SYNRECEIVED_H
#define	_SYNRECEIVED_H

#include <iostream>

#include "SimpleTCP.h"
#include "SimplePacket.h"
#include "Established.h"

using namespace std;


class SynReceived : public SimpleTCPState {

    virtual void receive(SimpleTCP* s, AddressPort& ap, SimplePacket& packet) {
        cout << "SynReceived: Receive" << endl;

        string message = (char*) packet.get_payload();
        if (message.compare("ACK")) {
            return;
        }
        s->set_state(new Established());
    }

    virtual void timeout(SimpleTCP* s) {

    }

    virtual void enter(SimpleTCP* s) {
        enter_state("SynReceived");
    }

    virtual void exit(SimpleTCP* s) {
        leave_state("SynReceived");
    }
};

#endif	/* _SYNRECEIVED_H */

