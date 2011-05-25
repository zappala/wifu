#include "protocol/TCPTahoe.h"

TCPTahoe::TCPTahoe(int protocol) : Protocol(protocol) {
    states_we_can_send_ack_.insert(type_name(Established));
    states_we_can_send_ack_.insert(type_name(FinWait1));
    states_we_can_send_ack_.insert(type_name(FinWait2));
    states_we_can_send_ack_.insert(type_name(CloseWait));
    states_we_can_send_ack_.insert(type_name(Closing));
    states_we_can_send_ack_.insert(type_name(LastAck));
    states_we_can_send_ack_.insert(type_name(TimeWait));
}

TCPTahoe::~TCPTahoe() {

}

TCPTahoe& TCPTahoe::instance() {
    static TCPTahoe instance_;
    return instance_;
}

void TCPTahoe::icontext_socket(SocketEvent* e) {
    //    cout << "TCPTahoe::icontext_socket()" << endl;
    Socket* s = e->get_socket();
    map_[s] = new TCPTahoeIContextContainer();

    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_socket(e);
    c->get_connection_manager()->icontext_socket(e);
    c->get_congestion_control()->icontext_socket(e);
}

void TCPTahoe::icontext_bind(BindEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_bind(e);
    c->get_connection_manager()->icontext_bind(e);
    c->get_congestion_control()->icontext_bind(e);
}

void TCPTahoe::icontext_listen(ListenEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_listen(e);
    c->get_connection_manager()->icontext_listen(e);
    c->get_congestion_control()->icontext_listen(e);
}

void TCPTahoe::icontext_receive_packet(NetworkReceivePacketEvent* e) {
//    cout << "TCPTahoe::icontext_receive_packet(): " << endl;

    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;
    TCPPacket* p = (TCPPacket*) e->get_packet();
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c->get_reliability();
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c->get_connection_manager();
//    cout << p->to_s() << endl;

    // validate any ack number
    if (p->is_tcp_ack() && !is_valid_ack_number(rc, p)) {
        cout << "INVALID ACK NUMBER" << endl;
        rc->icontext_receive_packet(e);
        return;
    }

    // validate sequence number
    // This is on page 69 of RFC 793
    // We add on the case where no context exists for us to check (RCV.NXT == 0)
    if (!is_valid_sequence_number(rc, p)) {
        // TODO: is this the correct check?
        cout << "INVALID SEQUENCE NUMBER" << endl;
//        cout << "Current state: " << cmc->get_state_name() << endl;
        if (states_we_can_send_ack_.contains(cmc->get_state_name())) {
            cout << "INVALID SEQUENCE NUMBER, SENDING ACK" << endl;
            // <editor-fold defaultstate="collapsed" desc="Dispatch ACK">
            TCPPacket* response = new TCPPacket();
            response->insert_tcp_header_option(new TCPTimestampOption());

            AddressPort* destination = s->get_remote_address_port();
            AddressPort* source = s->get_local_address_port();

            response->set_ip_destination_address_s(destination->get_address());
            response->set_ip_source_address_s(source->get_address());

            response->set_destination_port(destination->get_port());
            response->set_source_port(source->get_port());

            response->set_data((unsigned char*) "", 0);

            SendPacketEvent* event = new SendPacketEvent(s, response);
            dispatch(event);
            // </editor-fold>
        }
        return;
    }

    if (p->is_tcp_fin() && rc->get_rcv_wnd() < MAX_TCP_RECEIVE_WINDOW_SIZE) {
        c->set_saved_fin(e);
        return;
    }

    rc->icontext_receive_packet(e);
    cmc->icontext_receive_packet(e);
    c->get_congestion_control()->icontext_receive_packet(e);


    if (c->get_saved_close_event() && s->get_send_buffer().empty()) {
        cmc->icontext_close(c->get_saved_close_event());
        c->set_saved_close_event(0);
    }
}

void TCPTahoe::icontext_send_packet(SendPacketEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;
    TCPPacket* p = (TCPPacket*) e->get_packet();

    c->get_reliability()->icontext_send_packet(e);
    c->get_connection_manager()->icontext_send_packet(e);
    c->get_congestion_control()->icontext_send_packet(e);

//    cout << "TCPTahoe::icontext_send_packet(): " << endl;
//    cout << p->to_s() << endl;

    send_network_packet(e->get_socket(), p);
}

void TCPTahoe::icontext_connect(ConnectEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_connect(e);
    c->get_connection_manager()->icontext_connect(e);
    c->get_congestion_control()->icontext_connect(e);
}

void TCPTahoe::icontext_accept(AcceptEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_accept(e);
    c->get_connection_manager()->icontext_accept(e);
    c->get_congestion_control()->icontext_accept(e);
}

void TCPTahoe::icontext_new_connection_established(ConnectionEstablishedEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_new_connection_established(e);
    c->get_connection_manager()->icontext_new_connection_established(e);
    c->get_congestion_control()->icontext_new_connection_established(e);
}

void TCPTahoe::icontext_new_connection_initiated(ConnectionInitiatedEvent* e) {
    Socket* listening_socket = e->get_socket();
    Socket* new_socket = e->get_new_socket();

    TCPTahoeIContextContainer* listening_cc = map_.find(listening_socket)->second;
    map_[new_socket] = listening_cc;

    TCPTahoeIContextContainer* new_cc = new TCPTahoeIContextContainer();
    map_[listening_socket] = new_cc;

    new_cc->get_reliability()->icontext_new_connection_initiated(e);
    new_cc->get_connection_manager()->icontext_new_connection_initiated(e);
    new_cc->get_congestion_control()->icontext_new_connection_initiated(e);
}

void TCPTahoe::icontext_close(CloseEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;


    if (s->get_send_buffer().empty()) {
        c->get_connection_manager()->icontext_close(e);
    } else {
        c->set_saved_close_event(e);
    }

    ResponseEvent* response = new ResponseEvent(s, e->get_name(), e->get_map()[FILE_STRING]);
    response->put(RETURN_VALUE_STRING, Utils::itoa(0));
    response->put(ERRNO, Utils::itoa(0));
    dispatch(response);
}

void TCPTahoe::icontext_timer_fired_event(TimerFiredEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_timer_fired_event(e);
    c->get_connection_manager()->icontext_timer_fired_event(e);
    c->get_congestion_control()->icontext_timer_fired_event(e);
}

void TCPTahoe::icontext_resend_packet(ResendPacketEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_resend_packet(e);
    c->get_connection_manager()->icontext_resend_packet(e);
    c->get_congestion_control()->icontext_resend_packet(e);

    send_network_packet(e->get_socket(), e->get_packet());
}

void TCPTahoe::icontext_send(SendEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    if (is_room_in_send_buffer(e)) {
        save_in_buffer_and_send_events(e);
    } else {
        c->set_saved_send_event(e);
    }

    c->get_reliability()->icontext_send(e);
    c->get_connection_manager()->icontext_send(e);
    c->get_congestion_control()->icontext_send(e);
}

void TCPTahoe::icontext_receive(ReceiveEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_receive(e);
    c->get_connection_manager()->icontext_receive(e);
    c->get_congestion_control()->icontext_receive(e);
}

void TCPTahoe::icontext_receive_buffer_not_empty(ReceiveBufferNotEmptyEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_receive_buffer_not_empty(e);
    c->get_connection_manager()->icontext_receive_buffer_not_empty(e);
    c->get_congestion_control()->icontext_receive_buffer_not_empty(e);
}

void TCPTahoe::icontext_receive_buffer_not_full(ReceiveBufferNotFullEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c->get_reliability();

    if (c->get_saved_fin() && rc->get_rcv_wnd() < MAX_TCP_RECEIVE_WINDOW_SIZE) {
        dispatch(c->get_saved_fin());
    }

    c->get_reliability()->icontext_receive_buffer_not_full(e);
    c->get_connection_manager()->icontext_receive_buffer_not_full(e);
    c->get_congestion_control()->icontext_receive_buffer_not_full(e);
}

void TCPTahoe::icontext_send_buffer_not_empty(SendBufferNotEmptyEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_send_buffer_not_empty(e);
    c->get_connection_manager()->icontext_send_buffer_not_empty(e);
    c->get_congestion_control()->icontext_send_buffer_not_empty(e);
}

void TCPTahoe::icontext_send_buffer_not_full(SendBufferNotFullEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    SendEvent* saved_send_event = c->get_saved_send_event();

    if (saved_send_event && is_room_in_send_buffer(saved_send_event)) {
        save_in_buffer_and_send_events(saved_send_event);
    }

    c->get_reliability()->icontext_send_buffer_not_full(e);
    c->get_connection_manager()->icontext_send_buffer_not_full(e);
    c->get_congestion_control()->icontext_send_buffer_not_full(e);
}

bool TCPTahoe::icontext_can_send(Socket* s) {
    TCPTahoeIContextContainer* c = map_.find(s)->second;
    return c->get_connection_manager()->icontext_can_send(s);
}

bool TCPTahoe::icontext_can_receive(Socket* s) {
    TCPTahoeIContextContainer* c = map_.find(s)->second;
    return c->get_connection_manager()->icontext_can_receive(s);
}

void TCPTahoe::icontext_delete_socket(DeleteSocketEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_delete_socket(e);
    c->get_connection_manager()->icontext_delete_socket(e);
    c->get_congestion_control()->icontext_delete_socket(e);

    map_.erase(s);
    assert(map_.find(s) == map_.end());
}

void TCPTahoe::icontext_set_socket_option(SetSocketOptionEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_set_socket_option(e);
    c->get_connection_manager()->icontext_set_socket_option(e);
    c->get_congestion_control()->icontext_set_socket_option(e);
}

void TCPTahoe::icontext_get_socket_option(GetSocketOptionEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_get_socket_option(e);
    c->get_connection_manager()->icontext_get_socket_option(e);
    c->get_congestion_control()->icontext_get_socket_option(e);
}

bool TCPTahoe::is_room_in_send_buffer(SendEvent* e) {
    Socket* s = e->get_socket();
    int num_bytes_to_send = e->data_length();
    return s->get_send_buffer().size() + num_bytes_to_send <= MAX_BUFFER_SIZE;
}

void TCPTahoe::save_in_buffer_and_send_events(SendEvent* e) {
    int num_bytes_to_send = e->data_length();
    const char* data = (const char*) e->get_data();

    Socket* s = e->get_socket();
    s->get_send_buffer().append(data, num_bytes_to_send);

    ResponseEvent* response = new ResponseEvent(s, e->get_name(), e->get_map()[FILE_STRING]);
    response->put(RETURN_VALUE_STRING, Utils::itoa(num_bytes_to_send));
    response->put(ERRNO, Utils::itoa(0));

    dispatch(response);
    dispatch(new SendBufferNotEmptyEvent(s));
}

void TCPTahoe::create_and_dispatch_received_data(ReceiveEvent* e) {
    Socket* s = e->get_socket();
    int buffer_size = e->get_receive_buffer_size();

    string data = s->get_receive_buffer().substr(0, buffer_size);
    s->get_receive_buffer().erase(0, data.size());

    ResponseEvent* response = new ResponseEvent(s, e->get_name(), e->get_map()[FILE_STRING]);
    response->put(BUFFER_STRING, data);
    response->put(ADDRESS_STRING, s->get_remote_address_port()->get_address());
    response->put(PORT_STRING, Utils::itoa(s->get_remote_address_port()->get_port()));
    response->put(RETURN_VALUE_STRING, Utils::itoa(data.size()));
    response->put(ERRNO, Utils::itoa(0));

    dispatch(response);
    dispatch(new ReceiveBufferNotFullEvent(s));
}

bool TCPTahoe::is_valid_sequence_number(TCPTahoeReliabilityContext* rc, TCPPacket* p) {

    // This is the check to ensure we still continue if there is no context yet
    // It is not part of the RFC
    // TODO: this may need to change to something else as we may wrap around
    // I actually cannot remember why this needs to be here -- RB
    if (rc->get_rcv_nxt() == 0) {
        return true;
    }


    // These checks are in reverse order that they are on page 69
    // because I always seemed to get to the last one in tests
    if (p->get_data_length_bytes() > 0 && rc->get_rcv_wnd() > 0) {
        return between_equal_left(rc->get_rcv_nxt(), p->get_tcp_sequence_number(), rc->get_rcv_nxt() + rc->get_rcv_wnd()) ||
                between_equal_left(rc->get_rcv_nxt(), p->get_tcp_sequence_number() + p->get_data_length_bytes() - 1, rc->get_rcv_nxt() + rc->get_rcv_wnd());
    }

    if (p->get_data_length_bytes() > 0 && rc->get_rcv_wnd() == 0) {
        return false;
    }

    if (p->get_data_length_bytes() == 0 && rc->get_rcv_wnd() > 0) {
        return between_equal_left(rc->get_rcv_nxt(), p->get_tcp_sequence_number(), rc->get_rcv_nxt() + rc->get_rcv_wnd());
    }

    if (p->get_data_length_bytes() == 0 && rc->get_rcv_wnd() == 0) {
        return p->get_tcp_sequence_number() == rc->get_rcv_nxt();
    }

    return false;
}

bool TCPTahoe::is_valid_ack_number(TCPTahoeReliabilityContext* rc, TCPPacket* p) {
    //    cout << "TCPTahoe::is_valid_ack_number(), checking: " << rc->get_snd_una() << " <= " << p->get_tcp_ack_number() << " <= " << rc->get_snd_nxt() << endl;
    return between_equal(rc->get_snd_una(), p->get_tcp_ack_number(), rc->get_snd_nxt());
}