/* 
 * File:   ReliabilityState.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 3:40 PM
 */

#ifndef RELIABILITYSTATE_H
#define	RELIABILITYSTATE_H

#include <string>

#include "contexts/Context.h"
#include "contexts/ReliabilityContext.h"

#include "AddressPort.h"
#include "Socket.h"
#include "Dispatcher.h"

#include "packet/TCPPacket.h"

#include "events/ConnectEvent.h"
#include "events/TimerFiredEvent.h"
#include "events/TimerEvent.h"
#include "events/ResendPacketEvent.h"
#include "events/CancelTimerEvent.h"

#include "SimpleTCPCache.h"
#include "CacheMap.h"

using namespace std;

class ReliabilityState : public State {
public:
    ReliabilityState();
    virtual ~ReliabilityState();

    void receive_packet(Context* c, NetworkReceivePacketEvent* e);
    void enter(Context* c);
    void exit(Context* c);
    void send_packet(Context* c, SendPacketEvent* e);
    void timer_fired(Context* c, TimerFiredEvent* e);
};

#endif	/* RELIABILITYSTATE_H */

