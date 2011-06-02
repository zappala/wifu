/* 
 * File:   SlowStart.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 3:55 PM
 */

#ifndef SLOWSTART_H
#define	SLOWSTART_H

#include <string>

#include "contexts/Context.h"
#include "AddressPort.h"
#include "Socket.h"
#include "contexts/SimpleTCPCongestionControlContext.h"
#include "events/framework_events/NetworkSendPacketEvent.h"
#include "events/framework_events/ConnectEvent.h"
#include "events/protocol_events/SendPacketEvent.h"
#include "events/framework_events/NetworkReceivePacketEvent.h"
#include "events/framework_events/ResponseEvent.h"
#include "events/protocol_events/SendBufferNotEmptyEvent.h"
#include "Dispatcher.h"
#include "CacheMap.h"
#include "SimpleTCPCache.h"
#include "packet/TCPPacket.h"
#include "packet/TCPTimestampOption.h"

using namespace std;

class SimpleTCPCongestionControl : public State {
public:
    SimpleTCPCongestionControl();
    virtual ~SimpleTCPCongestionControl();

    void state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);
    void state_send_buffer_not_empty(Context* c, QueueProcessor<Event*>* q, SendBufferNotEmptyEvent* e);
    void state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e);
};


#endif	/* SLOWSTART_H */

