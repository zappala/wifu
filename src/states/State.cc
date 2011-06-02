/*
 * State.cc
 *
 *  Created on: Mar 5, 2011
 *      Author: erickson
 */

#include "states/State.h"

State::State() {}

State::~State() {}

void State::state_enter(Context* c) {}

void State::state_exit(Context* c) {}

void State::state_socket(Context* c, QueueProcessor<Event*>* q, SocketEvent* e) {}

void State::state_bind(Context* c, QueueProcessor<Event*>* q, BindEvent* e) {}

void State::state_listen(Context* c, QueueProcessor<Event*>* q, ListenEvent* e) {}

void State::state_accept(Context* c, QueueProcessor<Event*>* q, AcceptEvent* e) {}

void State::state_send(Context* c, QueueProcessor<Event*>* q, SendEvent* e) {}


bool State::state_can_receive(Context* c, Socket* s) {
    return false;
}

bool State::state_can_send(Context* c, Socket* s) {
    return false;
}

void State::state_receive(Context* c, QueueProcessor<Event*>* q, ReceiveEvent*) {}

void State::state_new_connection_established(Context* c, QueueProcessor<Event*>* q, ConnectionEstablishedEvent* e) {}

void State::state_new_connection_initiated(Context*, QueueProcessor<Event*>* q, ConnectionInitiatedEvent* e) {}

void State::state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {}

void State::state_timer_fired(Context* c, QueueProcessor<Event*>* q, TimerFiredEvent* e) {}

void State::state_connect(Context* c, QueueProcessor<Event*>* q, ConnectEvent* e) {}

void State::state_close(Context* c, QueueProcessor<Event*>* q, CloseEvent* e) {}

void State::state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e) {}

void State::state_resend_packet(Context*, QueueProcessor<Event*>* q, ResendPacketEvent* e) {}

void State::state_receive_buffer_not_empty(Context* c, QueueProcessor<Event*>* q, ReceiveBufferNotEmptyEvent* e) {}

void State::state_receive_buffer_not_full(Context* c, QueueProcessor<Event*>* q, ReceiveBufferNotFullEvent* e) {}

void State::state_send_buffer_not_empty(Context* c, QueueProcessor<Event*>* q, SendBufferNotEmptyEvent* e) {}

void State::state_send_buffer_not_full(Context* c, QueueProcessor<Event*>* q, SendBufferNotFullEvent* e) {}

void State::state_delete_socket(Context* c, QueueProcessor<Event*>* q, DeleteSocketEvent* e) {}

void State::state_set_socket_option(Context* c, QueueProcessor<Event*>* q, SetSocketOptionEvent* e) {}

void State::state_get_socket_option(Context* c, QueueProcessor<Event*>* q, GetSocketOptionEvent* e) {}