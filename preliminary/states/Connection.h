/* 
 * File:   Connection.h
 * Author: rbuck
 *
 * Created on September 28, 2010, 8:56 AM
 */

#ifndef _CONNECTION_H
#define	_CONNECTION_H

#include "Packet.h"

/**
 * Represents the context of the State Machine
 */
class Connection {
private:
    class TCPState * state;
public:
    Connection();
    virtual ~Connection();
    void setState(TCPState * newState);
};

class TCPState {
public:
    TCPState(){}
    virtual ~TCPState() {}

    /**
     * Called immediately following state assignment
     */
    virtual void enter(Connection * c) {}

    /**
     * Called immediately before we leave the current state
     */
    virtual void exit(Connection * c) {}

    
    virtual void receive(Connection * c, Packet * p) {}
    virtual void timeout(Connection * c) {}
    virtual void passiveOpen(Connection * c) {}
    virtual void activeOpen(Connection * c) {}
    virtual void close(Connection * c) {}
    virtual void send(Connection * c) {}
    virtual void transmit(Connection * c, Packet * p) {}
};



#endif	/* _CONNECTION_H */

