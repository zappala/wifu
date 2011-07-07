/*
 * State.h
 *
 *  Created on: Mar 5, 2011
 *      Author: erickson
 */

#ifndef STATE_H_
#define STATE_H_

#include "events/framework_events/SocketEvent.h"
#include "events/framework_events/BindEvent.h"
#include "events/framework_events/ListenEvent.h"
#include "events/framework_events/AcceptEvent.h"
#include "events/protocol_events/ConnectionEstablishedEvent.h"
#include "events/protocol_events/ConnectionInitiatedEvent.h"
#include "events/framework_events/NetworkReceivePacketEvent.h"
#include "events/framework_events/TimerFiredEvent.h"
#include "events/framework_events/ConnectEvent.h"
#include "events/protocol_events/SendPacketEvent.h"
#include "events/protocol_events/ResendPacketEvent.h"
#include "events/framework_events/CloseEvent.h"
#include "events/protocol_events/DeleteSocketEvent.h"
#include "events/protocol_events/ReceiveBufferNotFullEvent.h"
#include "events/framework_events/SetSocketOptionEvent.h"
#include "events/framework_events/GetSocketOptionEvent.h"

#include "contexts/Context.h"

#include "GarbageCollector.h"

using namespace std;

class Context;

/**
 * Base state class for use in the state pattern.
 * For information on the state pattern see http://sourcemaking.com/design_patterns/state which contains good example.
 *
 * This class has corresponding methods to those in IContext (and Context).
 * It is intended that this class receive requests from Context and process them as per the state pattern.
 * It is intended that this class be subclassed and that children classes only implement the functionallity needed.
 * Therefore, this class provides default (for the most part empty) methods.
 *
 * Child classes should hold the logic for the context to which it belongs.
 *
 * @see IContext
 * @see Context
 */
class State : public gc {
public:

    /**
     * Constructor.
     */
    State();

    /**
     * Destructor.
     */
    virtual ~State();

    /**
     * Called by a specific Context when we enter this state.
     * @param c The Context containing data and an interface to the outside.
     */
    virtual void state_enter(Context* c);

    /**
     * Called by a specific Context when we exit this state.
     * @param c The Context containing data and which called this method.
     */
    virtual void state_exit(Context* c);

    /**
     * Handles a request by Context::icontext_socket() when a SocketEvent occurs.
     *
     * @param c The Context containing data and which called this method.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The SocketEvent which caused this method to (eventually) be called.
     *
     * @see IContext::icontext_socket()
     * @see IContext
     * @see Context::icontext_socket()
     * @see Context
     */
    virtual void state_socket(Context* c, QueueProcessor<Event*>* q, SocketEvent* e);

    /**
     * Handles a request by Context::icontext_bind() when a BindEvent occurs.
     *
     * @param c The Context containing data and which called this method.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The BindEvent which caused this method to (eventually) be called.
     *
     * @see IContext::icontext_bind()
     * @see IContext
     * @see Context::icontext_bind()
     * @see Context
     */
    virtual void state_bind(Context* c, QueueProcessor<Event*>* q, BindEvent* e);

    /**
     * Handles a request by Context::icontext_listen() when a ListenEvent occurs.
     *
     * @param c The Context containing data and which called this method.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The ListenEvent which caused this method to (eventually) be called.
     *
     * @see IContext::icontext_listen()
     * @see IContext
     * @see Context::icontext_listen()
     * @see Context
     */
    virtual void state_listen(Context* c, QueueProcessor<Event*>* q, ListenEvent* e);

    /**
     * Handles a request by Context::icontext_connect() when a ConnectEvent occurs.
     *
     * @param c The Context containing data and which called this method.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The ConnectEvent which caused this method to (eventually) be called.
     *
     * @see IContext::icontext_connect()
     * @see IContext
     * @see Context::icontext_connect()
     * @see Context
     */
    virtual void state_connect(Context* c, QueueProcessor<Event*>* q, ConnectEvent* e);

    /**
     * Handles a request by Context::icontext_accept() when a AcceptEvent occurs.
     *
     * @param c The Context containing data and which called this method.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The AcceptEvent which caused this method to (eventually) be called.
     *
     * @see IContext::icontext_accept()
     * @see IContext
     * @see Context::icontext_accept()
     * @see Context
     */
    virtual void state_accept(Context* c, QueueProcessor<Event*>* q, AcceptEvent* e);

    /**
     * Handles a request by Context::icontext_new_connection_established() when a ConnectionEstablishedEvent occurs.
     *
     * @param c The Context containing data and which called this method.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The ConnectionEstablishedEvent which caused this method to (eventually) be called.
     *
     * @see IContext::icontext_new_connection_established()
     * @see IContext
     * @see Context::icontext_new_connection_established()
     * @see Context
     */
    virtual void state_new_connection_established(Context* c, QueueProcessor<Event*>* q, ConnectionEstablishedEvent* e);

    /**
     * Handles a request by Context::icontext_new_connection_initiated() when a ConnectionInitiatedEvent occurs.
     *
     * @param c The Context containing data and which called this method.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The ConnectionInitiatedEvent which caused this method to (eventually) be called.
     *
     * @see IContext::icontext_new_connection_initiated()
     * @see IContext
     * @see Context::icontext_new_connection_initiated()
     * @see Context
     */
    virtual void state_new_connection_initiated(Context* c, QueueProcessor<Event*>* q, ConnectionInitiatedEvent* e);

    /**
     * Handles a request by Context::icontext_close() when a CloseEvent occurs.
     *
     * @param c The Context containing data and which called this method.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The CloseEvent which caused this method to (eventually) be called.
     *
     * @see IContext::icontext_close()
     * @see IContext
     * @see Context::icontext_close()
     * @see Context
     */
    virtual void state_close(Context* c, QueueProcessor<Event*>* q, CloseEvent* e);

    /**
     * Handles a request by Context::icontext_receive_packet() when a NetworkReceivePacketEvent occurs.
     *
     * @param c The Context containing data and which called this method.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The NetworkReceivePacketEvent which caused this method to (eventually) be called.
     *
     * @see IContext::icontext_receive_packet()
     * @see IContext
     * @see Context::icontext_receive_packet()
     * @see Context
     */
    virtual void state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);

    /**
     * Handles a request by Context::icontext_send_packet() when a SendEvent occurs.
     *
     * @param c The Context containing data and which called this method.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The SendEvent which caused this method to (eventually) be called.
     *
     * @see IContext::icontext_send_packet()
     * @see IContext
     * @see Context::icontext_send_packet()
     * @see Context
     */
    virtual void state_send(Context* c, QueueProcessor<Event*>* q, SendEvent* e);

    /**
     * Handles a request by Context::icontext_timer_fired_event() when a TimerFiredEvent occurs.
     *
     * @param c The Context containing data and which called this method.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The TimerFiredEvent which caused this method to (eventually) be called.
     *
     * @see IContext::icontext_timer_fired_event()
     * @see IContext
     * @see Context::icontext_timer_fired_event()
     * @see Context
     */
    virtual void state_timer_fired(Context* c, QueueProcessor<Event*>* q, TimerFiredEvent* e);

    /**
     * Handles a request by Context::icontext_resend_packet() when a ResendPacketEvent occurs.
     *
     * @param c The Context containing data and which called this method.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The ResendPacketEvent which caused this method to (eventually) be called.
     *
     * @see IContext::icontext_resend_packet()
     * @see IContext
     * @see Context::icontext_resend_packet()
     * @see Context
     */
    virtual void state_resend_packet(Context* c, QueueProcessor<Event*>* q, ResendPacketEvent* e);

    /**
     * Handles a request by Context::icontext_send() when a SendPacketEvent occurs.
     *
     * @param c The Context containing data and which called this method.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The SendPacketEvent which caused this method to (eventually) be called.
     *
     * @see IContext::icontext_send()
     * @see IContext
     * @see Context::icontext_send()
     * @see Context
     */
    virtual void state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e);

    /**
     * Handles a request by Context::icontext_receive() when a ReceiveEvent occurs.
     *
     * @param c The Context containing data and which called this method.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The ReceiveEvent which caused this method to (eventually) be called.
     *
     * @see IContext::icontext_receive()
     * @see IContext
     * @see Context::icontext_receive()
     * @see Context
     */
    virtual void state_receive(Context* c, QueueProcessor<Event*>* q, ReceiveEvent* e);

    /**
     * Handles a request by Context::icontext_receive_buffer_not_empty() when a ReceiveBufferNotEmptyEvent occurs.
     *
     * @param c The Context containing data and which called this method.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The ReceiveBufferNotEmptyEvent which caused this method to (eventually) be called.
     *
     * @see IContext::icontext_receive_buffer_not_empty()
     * @see IContext
     * @see Context::icontext_receive_buffer_not_empty()
     * @see Context
     */
    virtual void state_receive_buffer_not_empty(Context* c, QueueProcessor<Event*>* q, ReceiveBufferNotEmptyEvent* e);

    /**
     * Handles a request by Context::icontext_receive_buffer_not_full() when a ReceiveBufferNotFullEvent occurs.
     *
     * @param c The Context containing data and which called this method.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The ReceiveBufferNotFullEvent which caused this method to (eventually) be called.
     *
     * @see IContext::icontext_receive_buffer_not_full()
     * @see IContext
     * @see Context::icontext_receive_buffer_not_full()
     * @see Context
     */
    virtual void state_receive_buffer_not_full(Context* c, QueueProcessor<Event*>* q, ReceiveBufferNotFullEvent* e);

    /**
     * Handles a request by Context::icontext_send_buffer_not_empty() when a SendBufferNotEmptyEvent occurs.
     *
     * @param c The Context containing data and which called this method.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The SendBufferNotEmptyEvent which caused this method to (eventually) be called.
     *
     * @see IContext::icontext_send_buffer_not_empty()
     * @see IContext
     * @see Context::icontext_send_buffer_not_empty()
     * @see Context
     */
    virtual void state_send_buffer_not_empty(Context* c, QueueProcessor<Event*>* q, SendBufferNotEmptyEvent* e);

    /**
     * Handles a request by Context::icontext_send_buffer_not_full() when a SendBufferNotFullEvent occurs.
     *
     * @param c The Context containing data and which called this method.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The SendBufferNotFullEvent which caused this method to (eventually) be called.
     *
     * @see IContext::icontext_send_buffer_not_full()
     * @see IContext
     * @see Context::icontext_send_buffer_not_full()
     * @see Context
     */
    virtual void state_send_buffer_not_full(Context* c, QueueProcessor<Event*>* q, SendBufferNotFullEvent* e);

    /**
     * Handles a request by Context::icontext_delete_socket() when a DeleteSocketEvent occurs.
     *
     * @param c The Context containing data and which called this method.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The DeleteSocketEvent which caused this method to (eventually) be called.
     *
     * @see IContext::icontext_delete_socket()
     * @see IContext
     * @see Context::icontext_delete_socket()
     * @see Context
     */
    virtual void state_delete_socket(Context* c, QueueProcessor<Event*>* q, DeleteSocketEvent* e);

    /**
     * Handles a request by Context::icontext_set_socket_option() when a SetSocketOptionEvent occurs.
     *
     * @param c The Context containing data and which called this method.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The SetSocketOptionEvent which caused this method to (eventually) be called.
     *
     * @see IContext::icontext_set_socket_option()
     * @see IContext
     * @see Context::icontext_set_socket_option()
     * @see Context
     */
    virtual void state_set_socket_option(Context* c, QueueProcessor<Event*>* q, SetSocketOptionEvent* e);

    /**
     * Handles a request by Context::icontext_get_socket_option() when a GetSocketOptionEvent occurs.
     *
     * @param c The Context containing data and which called this method.
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The GetSocketOptionEvent which caused this method to (eventually) be called.
     *
     * @see IContext::icontext_get_socket_option()
     * @see IContext
     * @see Context::icontext_get_socket_option()
     * @see Context
     */
    virtual void state_get_socket_option(Context* c, QueueProcessor<Event*>* q, GetSocketOptionEvent* e);

    /**
     * Handles a request by Context::icontext_can_send() to determine if s can send or not.
     *
     * @param c The Context containing data and which called this method.
     * @param s The socket to query on.
     * @return True if this IContext can receive data, false otherwise.
     * The default implementation always returns true.
     * Therefore, overriding methods only need to implement this method if they need to return true at some point.
     * 
     * @see IContext::icontext_can_send()
     * @see IContext
     * @see Context::icontext_can_send()
     * @see Context
     */
    virtual bool state_can_send(Context* c, Socket* s);

    /**
     * Handles a request by Context::icontext_can_receive() to determine if s can receive or not.
     *
     * @param c The Context containing data and which called this method.
     * @param s The socket to query on.
     * @return True if this IContext can receive data, false otherwise.
     * The default implementation always returns true.
     * Therefore, overriding methods only need to implement this method if they need to return true at some point.
     *
     * @see IContext::icontext_can_receive()
     * @see IContext
     * @see Context::icontext_can_receive()
     * @see Context
     */
    virtual bool state_can_receive(Context* c, Socket* s);
};

#endif /* STATE_H_ */
