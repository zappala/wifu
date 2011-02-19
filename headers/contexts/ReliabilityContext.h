/* 
 * File:   ReliabilityContext.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 10:36 AM
 */

#ifndef RELIABILITYCONTEXT_H
#define	RELIABILITYCONTEXT_H

#include "Context.h"
#include "states/ReliabilityState.h"
#include "AddressPort.h"
#include "Socket.h"
#include "events/ConnectEvent.h"
#include "events/AcceptEvent.h"
#include "packet/WiFuPacket.h"
#include "packet/TCPPacket.h"
#include "events/TimerFiredEvent.h"

class ReliabilityContext : public Context {
public:
    ReliabilityContext();
    virtual ~ReliabilityContext();
    void socket(Socket* s);
    void bind(Socket* s, AddressPort* ap);
    void listen(Socket* s, int back_log);
    void connect(ConnectEvent* e);
    void accept(AcceptEvent* e);
    void new_connection_established(Socket* s);
    void close();
    void receive_packet(Socket* s, WiFuPacket* p);
    void send_packet(Socket* s, WiFuPacket* p);
    void timer_fired_event(TimerFiredEvent* e);

    void set_saved_packet(TCPPacket* p);
    TCPPacket* get_saved_packet();

private:
    TCPPacket* saved_packet_;

};

#endif	/* RELIABILITYCONTEXT_H */

