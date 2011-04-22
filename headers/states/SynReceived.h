/* 
 * File:   SynReceived.h
 * Author: rbuck
 *
 * Created on December 31, 2010, 1:03 PM
 */

#ifndef SYNRECEIVED_H
#define	SYNRECEIVED_H

#include "contexts/Context.h"
#include "contexts/ConnectionManagerContext.h"
#include "states/Established.h"
#include "events/ConnectEvent.h"
#include "events/ConnectionEstablishedEvent.h"
#include "events/NetworkReceivePacketEvent.h"

using namespace std;

class SynReceived : public State {
public:
    SynReceived();
    virtual ~SynReceived();
    virtual void state_enter(Context* c);
    virtual void state_exit(Context* c);

    void state_receive_packet(Context* c, NetworkReceivePacketEvent* e);

};

#endif	/* SYNRECEIVED_H */

