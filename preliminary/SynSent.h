/* 
 * File:   SynSent.h
 * Author: rbuck
 *
 * Created on December 17, 2010, 1:12 PM
 */

#ifndef _SYNSENT_H
#define	_SYNSENT_H

#include <iostream>

#include "SimpleTCP.h"
#include "Established.h"

using namespace std;


class SynSent : public SimpleTCPState {

    virtual void receive(SimpleTCP* s, AddressPort& ap, SimplePacket& packet) {
        cout << "SynSent: Receive" << endl;

        string message = (char*) packet.get_payload();
        if (message.compare("SYNACK")) {
            cout << "SynSent: Returning early" << endl;
            return;
        }

        struct SimpleHeader header;
        header.ack = packet.get_header()->id;
        header.id = IDGenerator::instance().next();

        message = "ACK";
        SimplePacket p((unsigned char*) message.c_str(), message.length(), &header);
        s->send(&ap, p);

        s->set_state(new Established());
    }

    virtual void timeout(SimpleTCP* s) {

    }

    virtual void enter(SimpleTCP* s) {
        enter_state("SynSent");
    }

    virtual void exit(SimpleTCP* s) {
        leave_state("SynSent");
    }

};


#endif	/* _SYNSENT_H */

