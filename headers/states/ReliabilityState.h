/* 
 * File:   ReliabilityState.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 3:40 PM
 */

#ifndef RELIABILITYSTATE_H
#define	RELIABILITYSTATE_H

#include "../contexts/Context.h"
#include "../AddressPort.h"
#include "../Socket.h"
#include "../events/ConnectEvent.h"
#include <string>

using namespace std;

class ReliabilityState : public State {
public:
    ReliabilityState();
    virtual ~ReliabilityState();
    void receive(Context* c, Socket* s, WiFuPacket* p);
    void enter(Context* c);
    void exit(Context* c);
    void connect(Context* c, ConnectEvent* e);

};

#endif	/* RELIABILITYSTATE_H */

