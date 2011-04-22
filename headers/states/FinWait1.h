/* 
 * File:   FinWait1.h
 * Author: rbuck
 *
 * Created on April 18, 2011, 4:31 PM
 */

#ifndef FINWAIT1_H
#define	FINWAIT1_H


#include "contexts/Context.h"
#include "contexts/ConnectionManagerContext.h"

#include "events/NetworkReceivePacketEvent.h"
#include "packet/TCPPacket.h"
#include "Socket.h"

#include "states/State.h"
#include "states/FinWait2.h"
#include "states/Closing.h"

class FinWait1 : public State {
public:
    FinWait1();
    virtual ~FinWait1();
    virtual void state_enter(Context* c);
    virtual void state_exit(Context* c);

    virtual void close(Context* c);
    virtual void state_receive_packet(Context* c, NetworkReceivePacketEvent* e);

    virtual bool state_can_receive(Context* c, Socket* s);
};

#endif	/* FINWAIT1_H */

