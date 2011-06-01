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

#include "events/framework_events/ConnectEvent.h"
#include "events/framework_events/AcceptEvent.h"
#include "events/framework_events/TimerFiredEvent.h"
#include "events/framework_events/NetworkReceivePacketEvent.h"
#include "events/framework_events/SocketEvent.h"
#include "events/framework_events/BindEvent.h"
#include "events/framework_events/ListenEvent.h"
#include "events/framework_events/SendEvent.h"
#include "events/framework_events/ReceiveEvent.h"
#include "events/framework_events/CloseEvent.h"
#include "events/framework_events/GetSocketOptionEvent.h"
#include "events/framework_events/SetSocketOptionEvent.h"

#include "events/protocol_events/SendPacketEvent.h"
#include "events/protocol_events/ConnectionEstablishedEvent.h"
#include "events/protocol_events/ResendPacketEvent.h"
#include "events/protocol_events/ConnectionInitiatedEvent.h"
#include "events/protocol_events/SendBufferNotEmptyEvent.h"
#include "events/protocol_events/SendBufferNotFullEvent.h"
#include "events/protocol_events/ReceiveBufferNotEmptyEvent.h"
#include "events/protocol_events/ReceiveBufferNotFullEvent.h"
#include "events/protocol_events/DeleteSocketEvent.h"


using namespace std;

class IContext : public gc {
public:
    virtual void icontext_socket(SocketEvent* e) = 0;
    virtual void icontext_bind(BindEvent* e) = 0;
    virtual void icontext_listen(ListenEvent* e) = 0;

    virtual void icontext_receive_packet(NetworkReceivePacketEvent* e) = 0;
    virtual void icontext_send_packet(SendPacketEvent* e) = 0;

    virtual void icontext_connect(ConnectEvent* e) = 0;
    virtual void icontext_accept(AcceptEvent* e) = 0;
    virtual void icontext_new_connection_established(ConnectionEstablishedEvent* e) = 0;
    virtual void icontext_new_connection_initiated(ConnectionInitiatedEvent* e) = 0;
    virtual void icontext_close(CloseEvent*) = 0;

    virtual bool icontext_can_send(Socket*) = 0;
    virtual bool icontext_can_receive(Socket*) = 0;

    virtual void icontext_send(SendEvent* e) = 0;
    virtual void icontext_receive(ReceiveEvent* e) = 0;

    virtual void icontext_timer_fired_event(TimerFiredEvent* e) = 0;
    virtual void icontext_resend_packet(ResendPacketEvent* e) = 0;

    virtual void icontext_send_buffer_not_empty(SendBufferNotEmptyEvent*) = 0;
    virtual void icontext_send_buffer_not_full(SendBufferNotFullEvent*) = 0;

    virtual void icontext_receive_buffer_not_empty(ReceiveBufferNotEmptyEvent*) = 0;
    virtual void icontext_receive_buffer_not_full(ReceiveBufferNotFullEvent*) = 0;

    virtual void icontext_delete_socket(DeleteSocketEvent*) = 0;

    virtual void icontext_set_socket_option(SetSocketOptionEvent*) = 0;
    virtual void icontext_get_socket_option(GetSocketOptionEvent*) = 0;
};

#endif	/* ICONTEXT_H */
