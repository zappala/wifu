/* 
 * File:   CongestionControlContext.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 10:37 AM
 */

#ifndef CONGESTIONCONTROLCONTEXT_H
#define	CONGESTIONCONTROLCONTEXT_H

#include <string>
#include "Context.h"
#include "../states/SlowStart.h"
#include "../AddressPort.h"
#include "../Socket.h"
#include "../Packet.h"

using namespace std;

class CongestionControlContext : public Context {
public:

    CongestionControlContext();
    void listen(Socket* s, int back_log);
    void connect(Socket* s, AddressPort& dest);
    void close();
    void receive(string& data);
    void send(Socket* s, Packet* p);
    bool is_open();
};

#endif	/* CONGESTIONCONTROLCONTEXT_H */
