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

#include <string>

using namespace std;

class ReliabilityState : public State {
public:
    ReliabilityState();
    virtual ~ReliabilityState();
    void receive(Context* c, string& data);
    void enter(Context* c);
    void exit(Context* c);
    void connect(Context* c, Socket*s ,AddressPort& remote);

};

#endif	/* RELIABILITYSTATE_H */

