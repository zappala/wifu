/* 
 * File:   SimpleTCP.h
 * Author: rbuck
 *
 * Created on December 17, 2010, 12:31 PM
 */

#ifndef _SIMPLETCP_H
#define	_SIMPLETCP_H

#include <stdlib.h>
#include <iostream>

#include "../headers/UDPSocket.h"
#include "../headers/Identifiable.h"
#include "../headers/Queue.h"
#include "SimplePacket.h"

using namespace std;

class SimpleTCPState;

class SimpleTCP : public UDPSocketCallback {
public:

    SimpleTCP(AddressPort& ap);
    virtual ~SimpleTCP();

    void receive(AddressPort& ap, unsigned char* buffer, size_t length);
    void send(unsigned char* buffer, size_t length);
    void send(AddressPort* ap, SimplePacket& p);
    void send(SimplePacket& p);
    void connect(AddressPort& ap);
    void close();
    void set_state(SimpleTCPState* state);

    Semaphore& get_connected_flag();

private:
    UDPSocket socket_;

    // Local address and port that this object is bound to
    AddressPort* local_;

    // Remote address and port to which to send to
    AddressPort* remote_;

    // Queue to hold the packets
    Queue<SimplePacket*> queue_;

    Semaphore queue_flag_;

    class SimpleTCPState* state_;

    Semaphore connected_flag_;

    

};

class SimpleTCPState {
public:

    SimpleTCPState() {
    }

    virtual ~SimpleTCPState() {
    }

    /**
     * Called immediately following state assignment
     */
    virtual void enter(SimpleTCP * s) {
    }

    /**
     * Called immediately before we leave the current state
     */
    virtual void exit(SimpleTCP * s) {
    }

    virtual void receive(SimpleTCP * s, AddressPort& ap, SimplePacket& packet) {
    }

    virtual void timeout(SimpleTCP * s) {
    }

    virtual void activeOpen(SimpleTCP * s, AddressPort& ap) {
    }

    virtual void close(SimpleTCP * s) {
    }

    virtual void send(SimpleTCP * s) {
    }

    void enter_state(string state) {
        cout << "Entering " << state << " State" << endl;
    }

    void leave_state(string state) {
        cout << "Leaving " << state << " State" << endl;
    }
};

#endif	/* _SIMPLETCP_H */

