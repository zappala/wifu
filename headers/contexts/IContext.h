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

/**
 * Interface which is used by the Context's of each FSM in use.
 * It is also used by any class inheriting from Protocol.
 *
 * When Event::execute() is called, the event calls a specific Module::imodule_* method.
 * In the event that the Module is actually a Protocol, Protocol will call a corresponding IContext::icontext_* method.
 * Each Event object that might be destined for a Protocol should have a corresponding method in this interface.
 *
 * It is intended that every protocol implement this interface.
 * It is suggested that every protocol delegate as much work to finite state machines as possible.
 * This is the interface used as the finite state machine's context class.
 *
 * Context implements this interface and provides default implementations for all methods.
 *
 * For an example of how this is intended to be used, see TCPTahoe which subclasses Protocol and is therefore a IModule and an IContext.
 *
 * @see Context
 * @see IModule
 * @see Module
 * @see Protocol
 * @see TCPTahoe
 * @see Event
 */
class IContext : public gc {
public:

    /**
     * Called when a SocketEvent occurs and needs to be processed by a child of Protocol or by a FSM context.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The SocketEvent which caused this method to be called.
     * @see SocketEvent
     * @see IModule
     */
    virtual void icontext_socket(QueueProcessor<Event*>* q, SocketEvent* e) = 0;

    /**
     * Called when a BindEvent occurs and needs to be processed by a child of Protocol or by a FSM context.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The BindEvent which caused this method to be called.
     * @see BindEvent
     * @see IModule
     */
    virtual void icontext_bind(QueueProcessor<Event*>* q, BindEvent* e) = 0;

    /**
     * Called when a ListenEvent occurs and needs to be processed by a child of Protocol or by a FSM context.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The ListenEvent which caused this method to be called.
     * @see ListenEvent
     * @see IModule
     */
    virtual void icontext_listen(QueueProcessor<Event*>* q, ListenEvent* e) = 0;

    /**
     * Called when a NetworkReceivePacketEvent occurs and needs to be processed by a child of Protocol or by a FSM context.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The NetworkReceivePacketEvent which caused this method to be called.
     * @see NetworkReceivePacketEvent
     * @see IModule
     */
    virtual void icontext_receive_packet(QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) = 0;

    /**
     * Called when a SendPacketEvent occurs and needs to be processed by a child of Protocol or by a FSM context.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The SendPacketEvent which caused this method to be called.
     * @see SendPacketEvent
     * @see IModule
     */
    virtual void icontext_send_packet(QueueProcessor<Event*>* q, SendPacketEvent* e) = 0;

    /**
     * Called when a ConnectEvent occurs and needs to be processed by a child of Protocol or by a FSM context.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The ConnectEvent which caused this method to be called.
     * @see ConnectEvent
     * @see IModule
     */
    virtual void icontext_connect(QueueProcessor<Event*>* q, ConnectEvent* e) = 0;

    /**
     * Called when a AcceptEvent occurs and needs to be processed by a child of Protocol or by a FSM context.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The AcceptEvent which caused this method to be called.
     * @see AcceptEvent
     * @see IModule
     */
    virtual void icontext_accept(QueueProcessor<Event*>* q, AcceptEvent* e) = 0;

    /**
     * Called when a ConnectionEstablishedEvent occurs and needs to be processed by a child of Protocol or by a FSM context.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The ConnectionEstablishedEvent which caused this method to be called.
     * @see ConnectionEstablishedEvent
     * @see IModule
     */
    virtual void icontext_new_connection_established(QueueProcessor<Event*>* q, ConnectionEstablishedEvent* e) = 0;

    /**
     * Called when a ConnectionInitiatedEvent occurs and needs to be processed by a child of Protocol or by a FSM context.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The ConnectionInitiatedEvent which caused this method to be called.
     * @see ConnectionInitiatedEvent
     * @see IModule
     */
    virtual void icontext_new_connection_initiated(QueueProcessor<Event*>* q, ConnectionInitiatedEvent* e) = 0;

    /**
     * Called when a CloseEvent occurs and needs to be processed by a child of Protocol or by a FSM context.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The CloseEvent which caused this method to be called.
     * @see CloseEvent
     * @see IModule
     */
    virtual void icontext_close(QueueProcessor<Event*>* q, CloseEvent* e) = 0;

    /**
     * Called when a SendEvent occurs and needs to be processed by a child of Protocol or by a FSM context.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The SendEvent which caused this method to be called.
     * @see SendEvent
     * @see IModule
     */
    virtual void icontext_send(QueueProcessor<Event*>* q, SendEvent* e) = 0;

    /**
     * Called when a ReceiveEvent occurs and needs to be processed by a child of Protocol or by a FSM context.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The ReceiveEvent which caused this method to be called.
     * @see ReceiveEvent
     * @see IModule
     */
    virtual void icontext_receive(QueueProcessor<Event*>* q, ReceiveEvent* e) = 0;

    /**
     * Called when a TimerFiredEvent occurs and needs to be processed by a child of Protocol or by a FSM context.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The TimerFiredEvent which caused this method to be called.
     * @see TimerFiredEvent
     * @see IModule
     */
    virtual void icontext_timer_fired_event(QueueProcessor<Event*>* q, TimerFiredEvent* e) = 0;

    /**
     * Called when a ResendPacketEvent occurs and needs to be processed by a child of Protocol or by a FSM context.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The ResendPacketEvent which caused this method to be called.
     * @see ResendPacketEvent
     * @see IModule
     */
    virtual void icontext_resend_packet(QueueProcessor<Event*>* q, ResendPacketEvent* e) = 0;

    /**
     * Called when a SendBufferNotEmptyEvent occurs and needs to be processed by a child of Protocol or by a FSM context.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The SendBufferNotEmptyEvent which caused this method to be called.
     * @see SendBufferNotEmptyEvent
     * @see IModule
     */
    virtual void icontext_send_buffer_not_empty(QueueProcessor<Event*>* q, SendBufferNotEmptyEvent* e) = 0;

    /**
     * Called when a SendBufferNotFullEvent occurs and needs to be processed by a child of Protocol or by a FSM context.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The SendBufferNotFullEvent which caused this method to be called.
     * @see SendBufferNotFullEvent
     * @see IModule
     */
    virtual void icontext_send_buffer_not_full(QueueProcessor<Event*>* q, SendBufferNotFullEvent* e) = 0;

    /**
     * Called when a ReceiveBufferNotEmptyEvent occurs and needs to be processed by a child of Protocol or by a FSM context.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The ReceiveBufferNotEmptyEvent which caused this method to be called.
     * @see ReceiveBufferNotEmptyEvent
     * @see IModule
     */
    virtual void icontext_receive_buffer_not_empty(QueueProcessor<Event*>* q, ReceiveBufferNotEmptyEvent* e) = 0;

    /**
     * Called when a ReceiveBufferNotFullEvent occurs and needs to be processed by a child of Protocol or by a FSM context.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The ReceiveBufferNotFullEvent which caused this method to be called.
     * @see ReceiveBufferNotFullEvent
     * @see IModule
     */
    virtual void icontext_receive_buffer_not_full(QueueProcessor<Event*>* q, ReceiveBufferNotFullEvent* e) = 0;

    /**
     * Called when a DeleteSocketEvent occurs and needs to be processed by a child of Protocol or by a FSM context.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The DeleteSocketEvent which caused this method to be called.
     * @see DeleteSocketEvent
     * @see IModule
     */
    virtual void icontext_delete_socket(QueueProcessor<Event*>* q, DeleteSocketEvent* e) = 0;

    /**
     * Called when a SetSocketOptionEvent occurs and needs to be processed by a child of Protocol or by a FSM context.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The SetSocketOptionEvent which caused this method to be called.
     * @see SetSocketOptionEvent
     * @see IModule
     */
    virtual void icontext_set_socket_option(QueueProcessor<Event*>* q, SetSocketOptionEvent* e) = 0;

    /**
     * Called when a GetSocketOptionEvent occurs and needs to be processed by a child of Protocol or by a FSM context.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The GetSocketOptionEvent which caused this method to be called.
     * @see GetSocketOptionEvent
     * @see IModule
     */
    virtual void icontext_get_socket_option(QueueProcessor<Event*>* q, GetSocketOptionEvent* e) = 0;

    /**
     * Query function used to find out whether this IContext can send data.
     * It will likely by mainly used in a connection manager.
     * @param s The socket to query on.
     * @return True if this IContext can send data, false otherwise.
     */
    virtual bool icontext_can_send(Socket* s) = 0;

    /**
     * Query function used to find out whether this IContext can receive data.
     * It will likely by mainly used in a connection manager.
     * @param s The socket to query on.
     * @return True if this IContext can receive data, false otherwise.
     */
    virtual bool icontext_can_receive(Socket* s) = 0;
};

#endif	/* ICONTEXT_H */
