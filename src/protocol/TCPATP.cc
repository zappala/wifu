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
    cout << "\nTCPATP::icontext_socket()" << endl;

    Socket* s = e->get_socket();
    map_[s] = new ATPIContextContainer();

    BasicIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_socket(q, e);
    c->get_connection_manager()->icontext_socket(q, e);
    c->get_congestion_control()->icontext_socket(q, e);
}

void TCPATP::icontext_receive_packet(QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    cout << "\nTCPATP::icontext_receive_packet()" << endl;

    Socket* s = e->get_socket();
    TCPPacket* p = (TCPPacket*) e->get_packet();

    TCPTahoeIContextContainer* c = map_.find(s)->second;
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c->get_reliability();
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c->get_connection_manager();

    if(!p->is_valid_tcp_checksum()) {
    	cout << "INVALID CHECKSUM" << endl;
        return;
    }

    // validate any ack number
    if (p->is_tcp_ack() && !is_valid_ack_number(rc, p)) {
        cout << "INVALID ACK NUMBER" << endl;
        rc->icontext_receive_packet(q, e);
        return;
    }



    // TODO: This is copied from Randy's TCPTahoe code. Maybe come up with better way.
    // validate sequence number
    // This is on page 69 of RFC 793
    // We add on the case where no context exists for us to check (RCV.NXT == 0)
    if (!is_valid_sequence_number(rc, p)) {
        // TODO: is this the correct check?
        cout << "INVALID SEQUENCE NUMBER" << endl;

        // See my notes for May 25, 2011 for why this must be - RB
        if (!strcmp(cmc->get_state_name().c_str(), type_name(TimeWait))) {
            cmc->icontext_receive_packet(q, e);
        }
        else if (states_we_can_send_ack_.contains(cmc->get_state_name())) {
            //cout << "INVALID SEQUENCE NUMBER, SENDING ACK" << endl;

            TCPPacket* response = new ATPPacket();
            response->insert_tcp_header_option(new TCPTimestampOption());

            AddressPort* destination = s->get_remote_address_port();
            AddressPort* source = s->get_local_address_port();

            response->set_ip_destination_address_s(destination->get_address());
            response->set_ip_source_address_s(source->get_address());

            response->set_destination_port(destination->get_port());
            response->set_source_port(source->get_port());

            response->set_data((unsigned char*) "", 0);

            SendPacketEvent* event = new SendPacketEvent(s, response);
            q->enqueue(event);
        }

        return;
    }

    // TODO: do we really want to hang on to the FIN packet?
    // We must ensure that we will get everything out of the receive buffer
    // before the FIN sender times out and resends the FIN
    // I guess we could simply cache it again if we are not ready to close???
    // See my notes on May 25, 2011 -RB
    if (p->is_tcp_fin() && rc->get_rcv_wnd() < MAX_TCP_RECEIVE_WINDOW_SIZE) {
        //        cout << "Saving FIN" << endl;
        c->set_saved_fin(e);
        return;
    }

    rc->icontext_receive_packet(q, e);
    cmc->icontext_receive_packet(q, e);
    c->get_congestion_control()->icontext_receive_packet(q, e);


    if (c->get_saved_close_event() && s->get_send_buffer().empty() && !c->get_saved_send_event()) {
        cmc->icontext_close(q, c->get_saved_close_event());
        c->set_saved_close_event(0);
    }

}

void TCPATP::icontext_send_packet(QueueProcessor<Event*>* q, SendPacketEvent* e) {
    cout << "\nTCPATP::icontext_send_packet()" << endl;

    Socket* s = e->get_socket();
    BasicIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_send_packet(q, e);
    c->get_connection_manager()->icontext_send_packet(q, e);

    // Taking care of sending actual packet
    c->get_congestion_control()->icontext_send_packet(q, e);

}

void TCPATP::icontext_new_connection_initiated(QueueProcessor<Event*>* q, ConnectionInitiatedEvent* e){
    cout << "\nTCPATP::icontext_new_connection_initiated()" << endl;

    Socket* listening_socket = e->get_socket();
    Socket* new_socket = e->get_new_socket();

    BasicIContextContainer* listening_cc = map_.find(listening_socket)->second;
    map_[new_socket] = listening_cc;

    BasicIContextContainer* new_cc = new ATPIContextContainer();
    map_[listening_socket] = new_cc;

    new_cc->get_reliability()->icontext_new_connection_initiated(q, e);
    new_cc->get_connection_manager()->icontext_new_connection_initiated(q, e);
    new_cc->get_congestion_control()->icontext_new_connection_initiated(q, e);
}

