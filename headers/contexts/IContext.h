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

#include "QueueProcessor.h"

using namespace std;

class IContext : public gc {
public:
    virtual void icontext_socket(QueueProcessor<Event*>* q, SocketEvent* e) = 0;
    virtual void icontext_bind(QueueProcessor<Event*>* q, BindEvent* e) = 0;
    virtual void icontext_listen(QueueProcessor<Event*>* q, ListenEvent* e) = 0;

    virtual void icontext_receive_packet(QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) = 0;
    virtual void icontext_send_packet(QueueProcessor<Event*>* q, SendPacketEvent* e) = 0;

    virtual void icontext_connect(QueueProcessor<Event*>* q, ConnectEvent* e) = 0;
    virtual void icontext_accept(QueueProcessor<Event*>* q, AcceptEvent* e) = 0;
    virtual void icontext_new_connection_established(QueueProcessor<Event*>* q, ConnectionEstablishedEvent* e) = 0;
    virtual void icontext_new_connection_initiated(QueueProcessor<Event*>* q, ConnectionInitiatedEvent* e) = 0;
    virtual void icontext_close(QueueProcessor<Event*>* q, CloseEvent* e) = 0;

    virtual bool icontext_can_send(Socket* s) = 0;
    virtual bool icontext_can_receive(Socket* s) = 0;

    virtual void icontext_send(QueueProcessor<Event*>* q, SendEvent* e) = 0;
    virtual void icontext_receive(QueueProcessor<Event*>* q, ReceiveEvent* e) = 0;

    virtual void icontext_timer_fired_event(QueueProcessor<Event*>* q, TimerFiredEvent* e) = 0;
    virtual void icontext_resend_packet(QueueProcessor<Event*>* q, ResendPacketEvent* e) = 0;

    virtual void icontext_send_buffer_not_empty(QueueProcessor<Event*>* q, SendBufferNotEmptyEvent* e) = 0;
    virtual void icontext_send_buffer_not_full(QueueProcessor<Event*>* q, SendBufferNotFullEvent* e) = 0;

    virtual void icontext_receive_buffer_not_empty(QueueProcessor<Event*>* q, ReceiveBufferNotEmptyEvent* e) = 0;
    virtual void icontext_receive_buffer_not_full(QueueProcessor<Event*>* q, ReceiveBufferNotFullEvent* e) = 0;

    virtual void icontext_delete_socket(QueueProcessor<Event*>* q, DeleteSocketEvent* e) = 0;

    virtual void icontext_set_socket_option(QueueProcessor<Event*>* q, SetSocketOptionEvent* e) = 0;
    virtual void icontext_get_socket_option(QueueProcessor<Event*>* q, GetSocketOptionEvent* e) = 0;
};

#endif	/* ICONTEXT_H */
