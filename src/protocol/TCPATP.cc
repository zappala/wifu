/*
 * TCP_ATP.cc
 *
 *  Created on: Jun 22, 2011
 *      Author: philipbl
 */

#include "../../headers/protocol/TCPATP.h"

TCPATP::TCPATP(int protocol) : TCPTahoe(protocol){

}

TCPATP::~TCPATP() {
}

TCPATP& TCPATP::instance() {
    static TCPATP instance_;
    return instance_;
}


void TCPATP::icontext_socket(QueueProcessor<Event*>* q, SocketEvent* e){
    cout << "TCPATP::icontext_socket() : entered" << endl;

    Socket* s = e->get_socket();
    map_[s] = new ATPIContextContainer();

    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_socket(q, e);
    c->get_connection_manager()->icontext_socket(q, e);
    c->get_congestion_control()->icontext_socket(q, e);
}

void TCPATP::icontext_send_packet(QueueProcessor<Event*>* q, SendPacketEvent* e) {
    cout << "TCPATP::icontext_send_packet() : entered" << endl;

    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_send_packet(q, e);
    c->get_connection_manager()->icontext_send_packet(q, e);

    // Taking care of sending actual packet
    c->get_congestion_control()->icontext_send_packet(q, e);

    /**
     *
     * TODO: MAKE SURE YOU FIND ALL TCPPACKET AND REPLACE IT WITH ATPPACKET
     *
     */

}

void TCPATP::icontext_new_connection_initiated(QueueProcessor<Event*>* q, ConnectionInitiatedEvent* e){
    cout << "TCPATP::icontext_new_connection_initiated() : entered" << endl;

    Socket* listening_socket = e->get_socket();
    Socket* new_socket = e->get_new_socket();

    TCPTahoeIContextContainer* listening_cc = map_.find(listening_socket)->second;
    map_[new_socket] = listening_cc;

    TCPTahoeIContextContainer* new_cc = new ATPIContextContainer();
    map_[listening_socket] = new_cc;

    new_cc->get_reliability()->icontext_new_connection_initiated(q, e);
    new_cc->get_connection_manager()->icontext_new_connection_initiated(q, e);
    new_cc->get_congestion_control()->icontext_new_connection_initiated(q, e);
}

