/* 
 * File:   FinWait1.h
 * Author: rbuck
 *
 * Created on April 18, 2011, 4:31 PM
 */

#ifndef FINWAIT1_H
#define	FINWAIT1_H

#include "State.h"
#include "contexts/Context.h"
#include "events/NetworkReceivePacketEvent.h"
#include "packet/TCPPacket.h"
#include "Socket.h"
#include "contexts/ConnectionManagerContext.h"


class FinWait1 : public State {
public:
    FinWait1();
    virtual ~FinWait1();
    virtual void enter(Context* c);
    virtual void exit(Context* c);

    virtual void close(Context* c);
    virtual void receive_packet(Context* c, NetworkReceivePacketEvent* e);
};

#endif	/* FINWAIT1_H */
