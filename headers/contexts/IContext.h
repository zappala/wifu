/* 
 * File:   IContext.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 12:02 PM
 */

#ifndef ICONTEXT_H
#define	ICONTEXT_H

#include <string>
#include "../GarbageCollector.h"

#include "../headers/Socket.h"
#include "../headers/AddressPort.h"
#include "../headers/Packet.h"

using namespace std;

class IContext : public gc {
public:
    virtual void listen(Socket* s, int back_log) = 0;

    
    virtual void receive(string& data) = 0;
    virtual void send(Socket* s, Packet* p) = 0;
    virtual void connect(Socket* s, AddressPort& dest) = 0;
    virtual void close() = 0;
    virtual bool is_open() = 0;
};

#endif	/* ICONTEXT_H */



