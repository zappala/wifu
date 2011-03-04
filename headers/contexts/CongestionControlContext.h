/* 
 * File:   CongestionControlContext.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 10:37 AM
 */

#ifndef CONGESTIONCONTROLCONTEXT_H
#define	CONGESTIONCONTROLCONTEXT_H

#include <string>
#include "Context.h"
#include "states/SlowStart.h"
#include "AddressPort.h"
#include "Socket.h"
#include "packet/WiFuPacket.h"
#include "events/ConnectEvent.h"
#include "events/AcceptEvent.h"
#include "events/TimerFiredEvent.h"

using namespace std;

class CongestionControlContext : public Context {
public:
    CongestionControlContext();
    void socket(SocketEvent* e);
    void bind(BindEvent* e);
    void listen(ListenEvent* e);
    void connect(ConnectEvent* e);
    void accept(AcceptEvent* e);
    void new_connection_established(ConnectionEstablishedEvent* e);
    void close();
    void receive_packet(NetworkReceivePacketEvent* e);
    void send_packet(SendPacketEvent* e);
    void timer_fired_event(TimerFiredEvent* e);
    void resend_packet(ResendPacketEvent* e);

};

#endif	/* CONGESTIONCONTROLCONTEXT_H */
