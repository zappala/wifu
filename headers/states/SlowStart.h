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
#include "contexts/CongestionControlContext.h"
#include "events/NetworkSendPacketEvent.h"
#include "events/ConnectEvent.h"
#include "events/SendPacketEvent.h"
#include "events/NetworkReceivePacketEvent.h"
#include "events/ResponseEvent.h"
#include "events/SendBufferNotEmptyEvent.h"
#include "Dispatcher.h"
#include "CacheMap.h"
#include "SimpleTCPCache.h"
#include "packet/TCPPacket.h"
#include "packet/TCPTimestampOption.h"

using namespace std;

class SlowStart : public State {
public:
    SlowStart();
    virtual ~SlowStart();

    void state_receive_packet(Context* c, NetworkReceivePacketEvent* e);
    void state_send_buffer_not_empty(Context* c, SendBufferNotEmptyEvent* e);
    void state_send_packet(Context* c, SendPacketEvent* e);
};


#endif	/* SLOWSTART_H */

