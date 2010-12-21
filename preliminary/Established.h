/* 
 * File:   Established.h
 * Author: rbuck
 *
 * Created on December 17, 2010, 3:22 PM
 */

#ifndef _ESTABLISHED_H
#define	_ESTABLISHED_H

#include <iostream>

using namespace std;

class Established : public SimpleTCPState {

    virtual void receive(SimpleTCP* s, AddressPort& ap, SimplePacket& packet) {
        cout << "Established: Receive" << endl;
        cout << "ID of packet: " << packet.get_header()->id << endl;
        cout << "Payload: " << (char*) packet.get_payload() << endl;


    }

    virtual void timeout(SimpleTCP* s) {

    }

    virtual void close(SimpleTCP* s) {
        cout << "Established: Close" << endl;

        struct SimpleHeader header;
        header.ack = 0;
        header.id = IDGenerator::instance().get();

        string message = "FIN";
        SimplePacket p((unsigned char*) message.c_str(), message.length(), &header);
        s->send(p);

        
        //s->set_state(new Closed());
    }

    virtual void enter(SimpleTCP* s) {
        enter_state("Established");
        s->get_connected_flag().post();
    }

    virtual void exit(SimpleTCP* s) {
        leave_state("Established");
    }

};

#endif	/* _ESTABLISHED_H */

