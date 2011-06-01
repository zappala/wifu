/* 
 * File:   FinWait2.h
 * Author: rbuck
 *
 * Created on April 18, 2011, 4:35 PM
 */

#ifndef FINWAIT2_H
#define	FINWAIT2_H

#include "State.h"
#include "contexts/Context.h"
#include "events/framework_events/NetworkReceivePacketEvent.h"
#include "packet/TCPPacket.h"
#include "Socket.h"
#include "contexts/ConnectionManagerContext.h"

#include "states/TimeWait.h"


class FinWait2 : public State {
public:
    FinWait2();
    virtual ~FinWait2();
    virtual void state_enter(Context* c);
    virtual void state_exit(Context* c);

    virtual void state_receive_packet(Context* c, NetworkReceivePacketEvent* e);
    virtual bool state_can_receive(Context* c, Socket* s);
};

#endif	/* FINWAIT2_H */

