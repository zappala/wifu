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
#include "contexts/SimpleTCPReliabilityContext.h"

#include "AddressPort.h"
#include "Socket.h"
#include "Dispatcher.h"

#include "packet/TCPPacket.h"

#include "events/framework_events/ConnectEvent.h"
#include "events/framework_events/TimerFiredEvent.h"
#include "events/framework_events/TimerEvent.h"
#include "events/protocol_events/ResendPacketEvent.h"
#include "events/framework_events/CancelTimerEvent.h"

#include "SimpleTCPCache.h"
#include "CacheMap.h"
#include "HashSet.h"
#include "packet/TCPTimestampOption.h"

using namespace std;

class SimpleTCPReliability : public State {
public:
    SimpleTCPReliability();
    virtual ~SimpleTCPReliability();

    void state_receive_packet(Context* c, NetworkReceivePacketEvent* e);
    void state_send_packet(Context* c, SendPacketEvent* e);
    void state_timer_fired(Context* c, TimerFiredEvent* e);

};

#endif	/* RELIABILITYSTATE_H */

