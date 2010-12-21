/* 
 * File:   Closed.h
 * Author: rbuck
 *
 * Created on December 17, 2010, 12:52 PM
 */

#ifndef _CLOSED_H
#define	_CLOSED_H

#include <iostream>
#include "SimpleTCP.h"
#include "SynSent.h"
#include "SynReceived.h"

using namespace std;

class Closed : public SimpleTCPState {
public:

    virtual void activeOpen(SimpleTCP* s, AddressPort& ap) {
        cout << "Closed: Active Open" << endl;

        struct SimpleHeader header;
        header.ack = 0;
        header.id = IDGenerator::instance().get();
        
        string message = "SYN";
        SimplePacket p((unsigned char*)message.c_str(), message.length(), &header);

        s->send(&ap, p);

        s->set_state(new SynSent());
    }

    virtual void receive(SimpleTCP* s, AddressPort& ap, SimplePacket& packet) {
        cout << "Closed: Receive" << endl;

        string message = (char*)packet.get_payload();
        if(message.compare("SYN")) {
            cout << "Closed: Returning early" << endl;
            return;
        }

        struct SimpleHeader header;
        header.ack = packet.get_header()->id;
        header.id = IDGenerator::instance().get();

        message = "SYNACK";
        SimplePacket p((unsigned char*)message.c_str(), message.length(), &header);
        s->send(&ap, p);
        s->set_state(new SynReceived());
        
    }

    virtual void enter(SimpleTCP* s) {
        enter_state("Closed");
    }

    virtual void exit(SimpleTCP* s) {
        leave_state("Closed");
    }
};

#endif	/* _CLOSED_H */

