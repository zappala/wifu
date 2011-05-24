/* 
 * File:   DummyCongestionController.cc
 * Author: rbuck
 * 
 * Created on May 23, 2011, 2:59 PM
 */

#include "states/DummyCongestionController.h"
#include "contexts/TCPTahoeCongestionControlContext.h"
#include "packet/TCPPacket.h"

DummyCongestionController::DummyCongestionController() : State() {
}

DummyCongestionController::DummyCongestionController(const DummyCongestionController& orig) : State() {
}

DummyCongestionController::~DummyCongestionController() {
}

void DummyCongestionController::state_send_packet(Context* c, SendPacketEvent* e) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
    TCPPacket* p = (TCPPacket*) e->get_packet();

    if (p->is_tcp_syn()) {
        ccc->set_snd_una(ccc->get_iss());
        ccc->set_snd_nxt(ccc->get_snd_nxt() + 1);
    } else if (p->is_tcp_fin()) {
        ccc->set_snd_nxt(ccc->get_snd_nxt() + 1);
    }
    // we will set snd.nxt for data when we originally send data

}

void DummyCongestionController::state_resend_packet(Context* c, ResendPacketEvent* e) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
    TCPPacket* p = (TCPPacket*) e->get_packet();

    ccc->set_snd_nxt(ccc->get_snd_una() + p->get_data_length_bytes());

    // TODO: resize the window?
}

void DummyCongestionController::state_receive_packet(Context* c, NetworkReceivePacketEvent* e) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
    TCPPacket* p = (TCPPacket*) e->get_packet();

    if (p->is_tcp_ack() && between_equal_right(ccc->get_snd_una(), p->get_tcp_ack_number(), ccc->get_snd_nxt())) {
        ccc->set_snd_una(p->get_tcp_ack_number());
    }

    send_packets(c, e);
}

void DummyCongestionController::state_send_buffer_not_empty(Context* c, SendBufferNotEmptyEvent* e) {
    send_packets(c, e);
}

void DummyCongestionController::send_packets(Context* c, Event* e) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
    
    Socket* s = e->get_socket();
    
    
}

