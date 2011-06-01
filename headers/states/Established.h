/* 
 * File:   Established.h
 * Author: rbuck
 *
 * Created on December 31, 2010, 12:54 PM
 */

#ifndef ESTABLISHED_H
#define	ESTABLISHED_H

#include "State.h"


#include "contexts/Context.h"
#include "contexts/ConnectionManagerContext.h"

#include "states/FinWait1.h"
#include "states/CloseWait.h"

#include "events/framework_events/ResponseEvent.h"
#include "events/framework_events/ConnectEvent.h"
#include "events/framework_events/NetworkReceivePacketEvent.h"
#include "events/framework_events/CloseEvent.h"
#include "events/protocol_events/ReceiveBufferNotFullEvent.h"

#include "packet/TCPPacket.h"

using namespace std;

class Established : public State {
public:
    Established();
    virtual ~Established();
    virtual void state_enter(Context* c);
    virtual void state_exit(Context* c);

    void state_receive_packet(Context* c, NetworkReceivePacketEvent* e);
    bool state_can_receive(Context* c, Socket* s);
    bool state_can_send(Context* c, Socket* s);
    void state_close(Context* c, CloseEvent* e);
};

#endif	/* ESTABLISHED_H */

