/* 
 * File:   Closing.h
 * Author: rbuck
 *
 * Created on April 18, 2011, 4:36 PM
 */

#ifndef CLOSING_H
#define	CLOSING_H

#include "State.h"
#include "contexts/Context.h"
#include "events/NetworkReceivePacketEvent.h"
#include "packet/TCPPacket.h"
#include "Socket.h"
#include "contexts/ConnectionManagerContext.h"

class Closing : public State {
public:
    Closing();
    virtual ~Closing();
    virtual void enter(Context* c);
    virtual void exit(Context* c);

    virtual void receive_packet(Context* c, NetworkReceivePacketEvent* e);
};

#endif	/* CLOSING_H */

