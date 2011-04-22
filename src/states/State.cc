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

void State::state_socket(Context* c, SocketEvent* e) {}

void State::state_bind(Context* c, BindEvent* e) {}

void State::state_listen(Context* c, ListenEvent* e) {}

void State::state_accept(Context* c, AcceptEvent* e) {}

void State::state_send(Context*, SendEvent*) {}


bool State::state_can_receive(Context* c, Socket* s) {
    return false;
}

bool State::state_can_send(Context* c, Socket* s) {
    return false;
}

void State::state_receive(Context*, ReceiveEvent*) {}

void State::state_new_connection_established(Context* c, ConnectionEstablishedEvent* e) {}

void State::state_new_connection_initiated(Context*, ConnectionInitiatedEvent* e) {}

void State::state_receive_packet(Context* c, NetworkReceivePacketEvent* e) {}

void State::state_timer_fired(Context* c, TimerFiredEvent* e) {}

void State::state_connect(Context* c, ConnectEvent* e) {}

void State::state_close(Context* c, CloseEvent* e) {}

void State::state_send_packet(Context* c, SendPacketEvent* e) {}

void State::state_resend_packet(Context*, ResendPacketEvent* e) {}

void State::state_receive_buffer_not_empty(Context*, ReceiveBufferNotEmptyEvent*) {}

void State::state_receive_buffer_not_full(Context*, ReceiveBufferNotFullEvent*) {}

void State::state_send_buffer_not_empty(Context*, SendBufferNotEmptyEvent*) {}

void State::state_send_buffer_not_full(Context*, SendBufferNotFullEvent*) {}

void State::state_delete_socket(Context*, DeleteSocketEvent*) {}