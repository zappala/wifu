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

#include "states/Established.h"
#include "states/FinWait1.h"
#include "states/CloseWait.h"

#include "events/ResponseEvent.h"
#include "events/ConnectEvent.h"
#include "events/NetworkReceivePacketEvent.h"
#include "events/CloseEvent.h"

#include "packet/TCPPacket.h"

using namespace std;

class Established : public State {
public:
    Established();
    virtual ~Established();
    virtual void enter(Context* c);
    virtual void exit(Context* c);

    void receive_packet(Context* c, NetworkReceivePacketEvent* e);
    bool state_can_receive(Context* c, Socket* s);
    bool state_can_send(Context* c, Socket* s);
    void state_close(Context* c, CloseEvent* e);
};

#endif	/* ESTABLISHED_H */

