/* 
 * File:   IContext.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 12:02 PM
 */

#ifndef ICONTEXT_H
#define	ICONTEXT_H

#include <string>
#include "GarbageCollector.h"

#include "events/ConnectEvent.h"
#include "events/AcceptEvent.h"
#include "events/TimerFiredEvent.h"
#include "events/NetworkReceivePacketEvent.h"
#include "events/SocketEvent.h"
#include "events/BindEvent.h"
#include "events/ListenEvent.h"
#include "events/SendPacketEvent.h"
#include "events/ConnectionEstablishedEvent.h"
#include "events/ResendPacketEvent.h"

using namespace std;

class IContext : public gc {
public:
    virtual void socket(SocketEvent* e) = 0;
    virtual void bind(BindEvent* e) = 0;
    virtual void listen(ListenEvent* e) = 0;
    
    virtual void receive_packet(NetworkReceivePacketEvent* e) = 0;
    virtual void send_packet(SendPacketEvent* e) = 0;
    virtual void connect(ConnectEvent* e) = 0;
    virtual void accept(AcceptEvent* e) = 0;
    virtual void new_connection_established(ConnectionEstablishedEvent* e) = 0;
    virtual void close() = 0;

    virtual void timer_fired_event(TimerFiredEvent* e) = 0;
    virtual void resend_packet(ResendPacketEvent* e) = 0;
};

#endif	/* ICONTEXT_H */
