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
    void resend_packet(Context* c, ResendPacketEvent* e);

private:
    bool should_set_resend_timer(TCPPacket* p);
    void create_save_and_dispatch_timeout_event(Context* c, Socket* s, int seconds, long int nanoseconds);

    // private methods used in receive_packet()
    void cancel_timer(Context* c, WiFuPacket* p);
    bool check_and_resend_packet(Context* c, Socket* s, WiFuPacket* p);
};

#endif	/* RELIABILITYSTATE_H */

