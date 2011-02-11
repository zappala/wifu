/* 
 * File:   ReliabilityState.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 3:40 PM
 */

#ifndef RELIABILITYSTATE_H
#define	RELIABILITYSTATE_H

#include "contexts/Context.h"
#include "AddressPort.h"
#include "Socket.h"
#include "events/ConnectEvent.h"
#include <string>
#include "events/TimerFiredEvent.h"

using namespace std;

class ReliabilityState : public State {
public:
    ReliabilityState();
    virtual ~ReliabilityState();

    void receive_packet(Context* c, Socket* s, WiFuPacket* p);
    void enter(Context* c);
    void exit(Context* c);
    void send_packet(Context* c, Socket* s, WiFuPacket* p);
    void timer_fired(Context* c, TimerFiredEvent* e);
};

#endif	/* RELIABILITYSTATE_H */

