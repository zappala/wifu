/* 
 * File:   Established.h
 * Author: rbuck
 *
 * Created on December 17, 2010, 3:22 PM
 */

#ifndef _ESTABLISHED_H
#define	_ESTABLISHED_H

#include <iostream>

#include "SimpleTCP.h"

using namespace std;


class Established : public SimpleTCPState {

    virtual void receive(SimpleTCP* s, AddressPort& ap, SimplePacket& packet) {
        cout << "Established: Receive" << endl;
        cout << "ID of packet: " << packet.get_header()->id << endl;
        cout << "Payload: " << (char*) packet.get_payload() << endl;

        
    }

    virtual void timeout(SimpleTCP* s) {

    }

    virtual void enter(SimpleTCP* s) {
        enter_state("Established");
    }

    virtual void exit(SimpleTCP* s) {
        leave_state("Established");
    }

};

#endif	/* _ESTABLISHED_H */

