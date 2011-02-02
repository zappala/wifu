/* 
 * File:   IContext.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 12:02 PM
 */

#ifndef ICONTEXT_H
#define	ICONTEXT_H

#include <string>
#include "GarbageCollector.h"

#include "Socket.h"
#include "AddressPort.h"
#include "packet/WiFuPacket.h"
#include "events/ConnectEvent.h"
#include "events/AcceptEvent.h"

using namespace std;

class IContext : public gc {
public:
    virtual void listen(Socket* s, int back_log) = 0;
    
    virtual void receive(Socket* s, WiFuPacket* p) = 0;
    virtual void send(Socket* s, WiFuPacket* p) = 0;
    virtual void connect(ConnectEvent* e) = 0;
    virtual void accept(AcceptEvent* e) = 0;
    virtual void connection_established(Socket* s) = 0;
    virtual void close() = 0;
};

#endif	/* ICONTEXT_H */



