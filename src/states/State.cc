/*
 * State.cc
 *
 *  Created on: Mar 5, 2011
 *      Author: erickson
 */

#include "states/State.h"

State::State() {}

State::~State() {}

void State::enter(Context* c) {}

void State::exit(Context* c) {}

void State::socket(Context* c, SocketEvent* e) {}

void State::bind(Context* c, BindEvent* e) {}

void State::listen(Context* c, ListenEvent* e) {}

void State::accept(Context* c, AcceptEvent* e) {}

void State::new_connection_established(Context* c, ConnectionEstablishedEvent* e) {}

void State::new_connection_initiated(Context*, ConnectionInitiatedEvent* e) {}

void State::receive_packet(Context* c, NetworkReceivePacketEvent* e) {}

void State::timer_fired(Context* c, TimerFiredEvent* e) {}

void State::connect(Context* c, ConnectEvent* e) {}

void State::close(Context* c) {}

void State::send_packet(Context* c, SendPacketEvent* e) {}

void State::resend_packet(Context*, ResendPacketEvent* e) {}

void State::enter_state(string state) {
    //cout << "Entering " << state << " State" << endl;
}

void State::leave_state(string state) {
    //cout << "Leaving " << state << " State" << endl;
}
