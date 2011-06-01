/* 
 * File:   Accept.h
 * Author: rbuck
 *
 * Created on January 14, 2011, 10:42 AM
 */

#ifndef _ACCEPT_H
#define	_ACCEPT_H

#include "contexts/Context.h"
#include "contexts/ConnectionManagerContext.h"
#include "states/SynReceived.h"
#include "events/framework_events/ConnectEvent.h"
#include "events/framework_events/CloseEvent.h"
#include "states/Closed.h"
#include "SocketCollection.h"

using namespace std;

class Accept : public State {
public:
    Accept();
    virtual ~Accept();
    virtual void state_enter(Context* c);
    virtual void state_exit(Context* c);

    void state_receive_packet(Context* c, NetworkReceivePacketEvent* e);
    void state_close(Context* c, CloseEvent* e);
};

#endif	/* _ACCEPT_H */

