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
#include "events/SendBufferNotEmpty.h"
#include "Dispatcher.h"
#include "CacheMap.h"
#include "SimpleTCPCache.h"
#include "packet/TCPPacket.h"


using namespace std;

class SlowStart : public State {
public:
    SlowStart();
    virtual ~SlowStart();

    void send_packet(Context* c, SendPacketEvent* e);
    void receive_packet(Context* c, NetworkReceivePacketEvent* e);
    void enter(Context* c);
    void exit(Context* c);

    void send_to(Context* c, SendEvent* e);

    void state_send_buffer_not_empty(Context* c, SendBufferNotEmpty* e);

};


#endif	/* SLOWSTART_H */

