/* 
 * File:   Context.h
 * Author: rbuck
 *
 * Created on December 30, 2010, 10:36 AM
 */

#ifndef CONTEXT_H
#define	CONTEXT_H

#include <string>
#include <iostream>
#include "IContext.h"

#include "AddressPort.h"
#include "Socket.h"
#include "states/State.h"

using namespace std;

class State;

/**
 * Base context for all finite state machines in WiFu.
 * It is an interface to the world to access this FSM.
 * It keeps track of the current state and delegates requests to its current state.
 * This class delegates all responsibility to the current state's corresponding method.
 * It is up to the specific State implementations to handle any requests.
 * For information on the state pattern see http://sourcemaking.com/design_patterns/state which contains good examples.
 *
 * @see IContext
 * @see State
 */
class Context : public IContext {
public:

    /**
     * Constructor.
     * Sets the current state to null.
     */
    Context();

    /**
     * Destructor.
     */
    virtual ~Context();

    /**
     * Does three main things.
     * 1. Exits the current state if it exists.
     * 2. Saves the current state to s.
     * 3. Enters the current state if s is non-null.
     *
     * @param s The state to set and enter.
     */
    virtual void set_state(State* s);

    /**
     * @return A pointer to the current state.
     */
    virtual State* get_state();

    /**
     * @return A string representation of the current state.
     */
    virtual string get_state_name();

    /**
     * Delegates handling of this request to State::state_socket()
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The SocketEvent which caused this method to be called.
     *
     * @see IContext::icontext_socket()
     * @see IContext
     * @see State::state_socket()
     * @see State
     */
    virtual void icontext_socket(QueueProcessor<Event*>* q, SocketEvent* e);

    /**
     * Delegates handling of this request to State::state_bind()
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The BindEvent which caused this method to be called.
     *
     * @see IContext::icontext_bind()
     * @see IContext
     * @see State::state_bind()
     * @see State
     */
    virtual void icontext_bind(QueueProcessor<Event*>* q, BindEvent* e);

    /**
     * Delegates handling of this request to State::state_listen()
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The ListenEvent which caused this method to be called.
     *
     * @see IContext::icontext_listen()
     * @see IContext
     * @see State::state_listen()
     * @see State
     */
    virtual void icontext_listen(QueueProcessor<Event*>* q, ListenEvent* e);

    /**
     * Delegates handling of this request to State::state_connect()
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The ConnectEvent which caused this method to be called.
     *
     * @see IContext::icontext_connect()
     * @see IContext
     * @see State::state_connect()
     * @see State
     */
    virtual void icontext_connect(QueueProcessor<Event*>* q, ConnectEvent* e);

    /**
     * Delegates handling of this request to State::state_accept()
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The AcceptEvent which caused this method to be called.
     *
     * @see IContext::icontext_accept()
     * @see IContext
     * @see State::state_accept()
     * @see State
     */
    virtual void icontext_accept(QueueProcessor<Event*>* q, AcceptEvent* e);

    /**
     * Delegates handling of this request to State::state_new_connection_established()
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The ConnectionEstablishedEvent which caused this method to be called.
     *
     * @see IContext::icontext_new_connection_established()
     * @see IContext
     * @see State::state_new_connection_established()
     * @see State
     */
    virtual void icontext_new_connection_established(QueueProcessor<Event*>* q, ConnectionEstablishedEvent* e);

    /**
     * Delegates handling of this request to State::state_new_connection_initiated()
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The ConnectionInitiatedEvent which caused this method to be called.
     *
     * @see IContext::icontext_new_connection_initiated()
     * @see IContext
     * @see State::state_new_connection_initiated()
     * @see State
     */
    virtual void icontext_new_connection_initiated(QueueProcessor<Event*>* q, ConnectionInitiatedEvent* e);

    /**
     * Delegates handling of this request to State::state_close()
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The CloseEvent which caused this method to be called.
     *
     * @see IContext::icontext_close()
     * @see IContext
     * @see State::state_close()
     * @see State
     */
    virtual void icontext_close(QueueProcessor<Event*>* q, CloseEvent* e);

    /**
     * Delegates handling of this request to State::state_receive_packet()
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The NetworkReceivePacketEvent which caused this method to be called.
     *
     * @see IContext::icontext_receive_packet()
     * @see IContext
     * @see State::state_receive_packet()
     * @see State
     */
    virtual void icontext_receive_packet(QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e);

    /**
     * Delegates handling of this request to State::state_send_packet()
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The SendPacketEvent which caused this method to be called.
     *
     * @see IContext::icontext_send_packet()
     * @see IContext
     * @see State::state_send_packet()
     * @see State
     */
    virtual void icontext_send_packet(QueueProcessor<Event*>* q, SendPacketEvent* e);

    /**
     * Delegates handling of this request to State::state_timer_fired()
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The TimerFiredEvent which caused this method to be called.
     *
     * @see IContext::icontext_timer_fired_event()
     * @see IContext
     * @see State::state_timer_fired()
     * @see State
     */
    virtual void icontext_timer_fired_event(QueueProcessor<Event*>* q, TimerFiredEvent* e);

    /**
     * Delegates handling of this request to State::state_resend_packet()
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The ResendPacketEvent which caused this method to be called.
     *
     * @see IContext::icontext_resend_packet()
     * @see IContext
     * @see State::state_resend_packet()
     * @see State
     */
    virtual void icontext_resend_packet(QueueProcessor<Event*>* q, ResendPacketEvent* e);

    /**
     * Delegates handling of this request to State::state_send()
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The SendEvent which caused this method to be called.
     *
     * @see IContext::icontext_send()
     * @see IContext
     * @see State::state_send()
     * @see State
     */
    virtual void icontext_send(QueueProcessor<Event*>* q, SendEvent* e);

    /**
     * Delegates handling of this request to State::state_receive()
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The ReceiveEvent which caused this method to be called.
     *
     * @see IContext::icontext_receive()
     * @see IContext
     * @see State::state_receive()
     * @see State
     */
    virtual void icontext_receive(QueueProcessor<Event*>* q, ReceiveEvent* e);

    /**
     * Delegates handling of this request to State::state_receive_buffer_not_empty()
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The ReceiveBufferNotEmptyEvent which caused this method to be called.
     *
     * @see IContext::icontext_receive_buffer_not_empty()
     * @see IContext
     * @see State::state_receive_buffer_not_empty()
     * @see State
     */
    virtual void icontext_receive_buffer_not_empty(QueueProcessor<Event*>* q, ReceiveBufferNotEmptyEvent* e);

    /**
     * Delegates handling of this request to State::state_receive_buffer_not_full()
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The ReceiveBufferNotFullEvent which caused this method to be called.
     *
     * @see IContext::icontext_receive_buffer_not_full()
     * @see IContext
     * @see State::state_receive_buffer_not_full()
     * @see State
     */
    virtual void icontext_receive_buffer_not_full(QueueProcessor<Event*>* q, ReceiveBufferNotFullEvent* e);

    /**
     * Delegates handling of this request to State::state_send_buffer_not_empty()
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The SendBufferNotEmptyEvent which caused this method to be called.
     *
     * @see IContext::icontext_send_buffer_not_empty()
     * @see IContext
     * @see State::state_send_buffer_not_empty()
     * @see State
     */
    virtual void icontext_send_buffer_not_empty(QueueProcessor<Event*>* q, SendBufferNotEmptyEvent* e);

    /**
     * Delegates handling of this request to State::state_send_buffer_not_full()
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The SendBufferNotFullEvent which caused this method to be called.
     *
     * @see IContext::icontext_send_buffer_not_full()
     * @see IContext
     * @see State::state_send_buffer_not_full()
     * @see State
     */
    virtual void icontext_send_buffer_not_full(QueueProcessor<Event*>* q, SendBufferNotFullEvent* e);

    /**
     * Delegates handling of this request to State::state_delete_socket()
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The DeleteSocketEvent which caused this method to be called.
     *
     * @see IContext::icontext_delete_socket()
     * @see IContext
     * @see State::state_delete_socket()
     * @see State
     */
    virtual void icontext_delete_socket(QueueProcessor<Event*>* q, DeleteSocketEvent* e);

    /**
     * Delegates handling of this request to State::state_set_socket_option()
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The SetSocketOptionEvent which caused this method to be called.
     *
     * @see IContext::icontext_set_socket_option()
     * @see IContext
     * @see State::state_set_socket_option()
     * @see State
     */
    virtual void icontext_set_socket_option(QueueProcessor<Event*>* q, SetSocketOptionEvent* e);

    /**
     * Delegates handling of this request to State::state_get_socket_option()
     * @param q Pointer to the QueueProcessor which dequeued e.
     * @param e The GetSocketOptionEvent which caused this method to be called.
     *
     * @see IContext::icontext_get_socket_option()
     * @see IContext
     * @see State::state_get_socket_option()
     * @see State
     */
    virtual void icontext_get_socket_option(QueueProcessor<Event*>* q, GetSocketOptionEvent* e);

    /**
     * Delegates handling of this request to State::state_can_send()
     * @param s The socket to query on.
     * @return True if this IContext can send data, false otherwise.
     *
     * @see IContext::icontext_can_send()
     * @see IContext
     * @see State::state_can_send()
     * @see State
     */
    virtual bool icontext_can_send(Socket* s);

    /**
     * Delegates handling of this request to State::state_can_receive()
     * @param s The socket to query on.
     * @return True if this IContext can receive data, false otherwise.
     *
     * @see IContext::icontext_can_receive()
     * @see IContext
     * @see State::state_can_receive()
     * @see State
     */
    virtual bool icontext_can_receive(Socket*);

private:
    
    /**
     * Pointer to the current state of this finite state machine.
     */
    State* current_;

};

#endif	/* CONTEXT_H */
