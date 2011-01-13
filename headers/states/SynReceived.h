/* 
 * File:   SynReceived.h
 * Author: rbuck
 *
 * Created on December 31, 2010, 1:03 PM
 */

#ifndef SYNRECEIVED_H
#define	SYNRECEIVED_H

#include "../contexts/Context.h"
#include "contexts/ConnectionManagerContext.h"
#include "states/Established.h"
#include "../events/ConnectEvent.h"
using namespace std;

class SynReceived : public State {
public:
    SynReceived();
    virtual ~SynReceived();
    virtual void enter(Context* c);
    virtual void exit(Context* c);

    void receive(Context* c, Socket* s, Packet* p);

};

#endif	/* SYNRECEIVED_H */

