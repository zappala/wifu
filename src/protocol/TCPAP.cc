/* 
 * File:   TCPAP.cc
 * Author: leer
 * 
 * Created on June 28, 2011, 3:22 PM
 */

#include "protocol/TCPAP.h"

TCPAP::TCPAP(int protocol, IContextContainerFactory* factory) : TCPTahoe(protocol, factory) {
}

TCPAP::~TCPAP() {
}

TCPAP& TCPAP::instance() {
    static TCPAP instance_;
    return instance_;
}

void TCPAP::icontext_receive_packet(QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    //cout << "TCPAP::icontext_receive_packet()\n";
    this->TCPTahoe::icontext_receive_packet(q,e);
    //cout << "TCPAP::icontext_receive_packet() returned from calling superclass\n";
    Socket* s = e->get_socket();
    TCPAPIContextContainer* c = (TCPAPIContextContainer*) map_.find(s)->second;
    c->get_rate_limiter()->icontext_receive_packet(q, e);
}

void TCPAP::icontext_send_packet(QueueProcessor<Event*>* q, SendPacketEvent* e) {
    //cout << "TCPAP::icontext_send_packet()\n";
    Socket* s = e->get_socket();
    TCPAPIContextContainer* c = (TCPAPIContextContainer*)map_.find(s)->second;
    TCPPacket* p = (TCPPacket*) e->get_packet();
    //cout << "Packet contents: \n" << p->to_s_format() << endl << p->to_s() << endl;

    c->get_reliability()->icontext_send_packet(q, e);
    c->get_connection_manager()->icontext_send_packet(q, e);
    c->get_congestion_control()->icontext_send_packet(q, e);

    c->get_rate_limiter()->icontext_send_packet(q,e);
}

void TCPAP::icontext_timer_fired_event(QueueProcessor<Event*>* q, TimerFiredEvent* e) {
    Socket* s = e->get_socket();
    TCPAPIContextContainer* c = (TCPAPIContextContainer*)map_.find(s)->second;

    c->get_rate_limiter()->icontext_timer_fired_event(q, e);
    this->TCPTahoe::icontext_timer_fired_event(q, e);
}

void TCPAP::icontext_set_socket_option(QueueProcessor<Event*>* q, SetSocketOptionEvent* e) {
    Socket* s = e->get_socket();
    TCPAPIContextContainer* c = (TCPAPIContextContainer*)map_.find(s)->second;

    this->TCPTahoe::icontext_set_socket_option(q, e);

    c->get_rate_limiter()->icontext_set_socket_option(q,e);

}