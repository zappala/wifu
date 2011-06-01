/* 
 * File:   SynSent.h
 * Author: rbuck
 *
 * Created on December 31, 2010, 12:50 PM
 */

#ifndef SYNSENT_H
#define	SYNSENT_H

#include "contexts/Context.h"
#include "contexts/ConnectionManagerContext.h"
#include "Established.h"
#include "events/framework_events/ConnectEvent.h"
#include "Dispatcher.h"

using namespace std;

class SynSent : public State {
public:
    SynSent();
    virtual ~SynSent();
    virtual void state_enter(Context* c);
    virtual void state_exit(Context* c);

    void state_receive_packet(Context* c, NetworkReceivePacketEvent* e);

};

#endif	/* SYNSENT_H */

