#include "protocol/TCPTahoe.h"

TCPTahoe::TCPTahoe(int protocol, IContextContainerFactory* factory) : Protocol(protocol), factory_(factory) {
    states_we_can_send_ack_.insert(type_name(Established));
    states_we_can_send_ack_.insert(type_name(FinWait1));
    states_we_can_send_ack_.insert(type_name(FinWait2));
    states_we_can_send_ack_.insert(type_name(CloseWait));
    states_we_can_send_ack_.insert(type_name(Closing));
    states_we_can_send_ack_.insert(type_name(LastAck));
    states_we_can_send_ack_.insert(type_name(TimeWait));
}

TCPTahoe::~TCPTahoe() {

//    log_INFORMATIONAL("tahoe_send_events_size: ", pantheios::i(send_events_.size()), " tahoe_send_response_events_size: ", pantheios::i(send_response_events_.size()), " tahoe_send_response_sizes_size: ", pantheios::i(send_response_sizes_.size()));
//
//
//    while (!send_response_events_.empty()) {
//
//        int size = 1000;
//        char start[size];
//        char end[size];
//        char size_i[size];
//        memset(start, 0, size);
//        memset(end, 0, size);
//        memset(size_i, 0, size);
//
//        sprintf(start, "%llu", send_events_.front());
//        sprintf(end, "%llu", send_response_events_.front());
//        sprintf(size_i, "%u", send_response_sizes_.front());
//
//        basic_string<PAN_CHAR_T> start_s(start);
//        basic_string<PAN_CHAR_T> end_s(end);
//        basic_string<PAN_CHAR_T> size_s(size_i);
//
//        log_INFORMATIONAL("send_tahoe ", start_s, " ", end_s, " ", size_s);
//
//        send_events_.pop_front();
//        send_response_events_.pop_front();
//        send_response_sizes_.pop_front();
//    }


}

TCPTahoe& TCPTahoe::instance() {
    static TCPTahoe instance_;
    return instance_;
}

void TCPTahoe::icontext_socket(QueueProcessor<Event*>* q, SocketEvent* e) {
    Socket* s = e->get_socket();
    map_[s] = factory_->get_icontext_container();

    BasicIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_socket(q, e);
    c->get_connection_manager()->icontext_socket(q, e);
    c->get_congestion_control()->icontext_socket(q, e);
}

void TCPTahoe::icontext_bind(QueueProcessor<Event*>* q, BindEvent* e) {
    Socket* s = e->get_socket();
    BasicIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_bind(q, e);
    c->get_connection_manager()->icontext_bind(q, e);
    c->get_congestion_control()->icontext_bind(q, e);
}

void TCPTahoe::icontext_listen(QueueProcessor<Event*>* q, ListenEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = (TCPTahoeIContextContainer*) map_.find(s)->second;

    c->set_back_log(new BackLog(e->get_back_log()));

    c->get_reliability()->icontext_listen(q, e);
    c->get_connection_manager()->icontext_listen(q, e);
    c->get_congestion_control()->icontext_listen(q, e);
}

void TCPTahoe::icontext_receive_packet(QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {

    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = (TCPTahoeIContextContainer*) map_.find(s)->second;
    TCPPacket* p = (TCPPacket*) e->get_packet();
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c->get_reliability();
    ConnectionManagerContext* cmc = (ConnectionManagerContext*) c->get_connection_manager();

    if (!p->is_valid_tcp_checksum()) {
        return;
    }

    // validate any ack number
    if (p->is_tcp_ack() && !is_valid_ack_number(rc, p)) {
        rc->icontext_receive_packet(q, e);
        return;
    }

    // validate sequence number
    // This is on page 69 of RFC 793
    // We add on the case where no context exists for us to check (RCV.NXT == 0)
    if (!is_valid_sequence_number(rc, p)) {
        // See my notes for May 25, 2011 for why this must be - RB
        if (!strcmp(cmc->get_state_name().c_str(), type_name(TimeWait))) {
            cmc->icontext_receive_packet(q, e);
        } else
            if (states_we_can_send_ack_.contains(cmc->get_state_name())) {
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
            q->enqueue(event);
            // </editor-fold>
        }
        return;
    }

    // TODO: do we really want to hang on to the FIN packet?
    // We must ensure that we will get everything out of the receive buffer
    // before the FIN sender times out and resends the FIN
    // I guess we could simply cache it again if we are not ready to close???
    // See my notes on May 25, 2011 -RB
    if (p->is_tcp_fin() && rc->get_rcv_wnd() < MAX_TCP_RECEIVE_WINDOW_SIZE) {
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

void TCPTahoe::icontext_send_packet(QueueProcessor<Event*>* q, SendPacketEvent* e) {
    Socket* s = e->get_socket();
    BasicIContextContainer* c = map_.find(s)->second;
    TCPPacket* p = (TCPPacket*) e->get_packet();

    c->get_reliability()->icontext_send_packet(q, e);
    c->get_connection_manager()->icontext_send_packet(q, e);
    c->get_congestion_control()->icontext_send_packet(q, e);

    p->pack();
    p->calculate_and_set_tcp_checksum();
    send_network_packet(e->get_socket(), p);
}

void TCPTahoe::icontext_connect(QueueProcessor<Event*>* q, ConnectEvent* e) {
    Socket* s = e->get_socket();
    BasicIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_connect(q, e);
    c->get_connection_manager()->icontext_connect(q, e);
    c->get_congestion_control()->icontext_connect(q, e);
}

void TCPTahoe::icontext_accept(QueueProcessor<Event*>* q, AcceptEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = (TCPTahoeIContextContainer*) map_.find(s)->second;

    c->get_reliability()->icontext_accept(q, e);
    c->get_connection_manager()->icontext_accept(q, e);
    c->get_congestion_control()->icontext_accept(q, e);

    if (!c->get_back_log()->empty()) {
        send_accept_response(c, e);
    } else {
        c->set_saved_accept_event(e);
    }
}

void TCPTahoe::icontext_new_connection_established(QueueProcessor<Event*>* q, ConnectionEstablishedEvent* e) {
    // e->get_socket() is the new socket
    Socket* s = e->get_new_socket();
    TCPTahoeIContextContainer* c = (TCPTahoeIContextContainer*) map_.find(s)->second;
    
    c->get_back_log()->push(e);

    AcceptEvent* ae = c->get_saved_accept_event();

    if (ae) {
        send_accept_response(c, ae);
        c->set_saved_accept_event(0);
    }
}

void TCPTahoe::icontext_new_connection_initiated(QueueProcessor<Event*>* q, ConnectionInitiatedEvent* e) {
    Socket* listening_socket = e->get_socket();
    Socket* new_socket = e->get_new_socket();

    TCPTahoeIContextContainer* listening_cc = (TCPTahoeIContextContainer*) map_.find(listening_socket)->second;
    map_[new_socket] = listening_cc;

    TCPTahoeIContextContainer* new_cc = (TCPTahoeIContextContainer*) factory_->get_icontext_container();
    new_cc->set_back_log(listening_cc->get_back_log());
    new_cc->set_saved_accept_event(listening_cc->get_saved_accept_event());
    map_[listening_socket] = new_cc;

    new_cc->get_reliability()->icontext_new_connection_initiated(q, e);
    new_cc->get_connection_manager()->icontext_new_connection_initiated(q, e);
    new_cc->get_congestion_control()->icontext_new_connection_initiated(q, e);


    ConnectionManagerContext* sending_cmc = (ConnectionManagerContext*) listening_cc->get_connection_manager();
    sending_cmc->set_socket(new_socket);
}

void TCPTahoe::icontext_close(QueueProcessor<Event*>* q, CloseEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = (TCPTahoeIContextContainer*) map_.find(s)->second;

    if (s->get_send_buffer().empty() && !c->get_saved_send_event()) {
        c->get_connection_manager()->icontext_close(q, e);
    } else {
        c->set_saved_close_event(e);
    }

    ResponseEvent* response_event = ObjectPool<ResponseEvent>::instance().get();
    response_event->set_socket(s);
    response_event->set_message_type(e->get_message_type());
    response_event->set_fd(e->get_fd());
    response_event->set_return_value(0);
    response_event->set_errno(0);
    response_event->set_default_length();
    response_event->set_destination(e->get_source());
    dispatch(response_event);
}

void TCPTahoe::icontext_timer_fired_event(QueueProcessor<Event*>* q, TimerFiredEvent* e) {
    Socket* s = e->get_socket();
    BasicIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_timer_fired_event(q, e);
    c->get_connection_manager()->icontext_timer_fired_event(q, e);
    c->get_congestion_control()->icontext_timer_fired_event(q, e);
}

void TCPTahoe::icontext_resend_packet(QueueProcessor<Event*>* q, ResendPacketEvent* e) {
    Socket* s = e->get_socket();
    BasicIContextContainer* c = map_.find(s)->second;

    c->get_congestion_control()->icontext_resend_packet(q, e);
    c->get_reliability()->icontext_resend_packet(q, e);
    c->get_connection_manager()->icontext_resend_packet(q, e);
}

void TCPTahoe::icontext_send(QueueProcessor<Event*>* q, SendEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = (TCPTahoeIContextContainer*) map_.find(s)->second;

    int available_space = get_available_room_in_send_buffer(e);

    if (available_space > 0) {
        save_in_buffer_and_send_events(q, e, available_space);
    } else {
        c->set_saved_send_event(e);
    }

    c->get_reliability()->icontext_send(q, e);
    c->get_connection_manager()->icontext_send(q, e);
    c->get_congestion_control()->icontext_send(q, e);
}

void TCPTahoe::icontext_receive(QueueProcessor<Event*>* q, ReceiveEvent* e) {
    Socket* s = e->get_socket();
    BasicIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_receive(q, e);
    c->get_connection_manager()->icontext_receive(q, e);
    c->get_congestion_control()->icontext_receive(q, e);
}

void TCPTahoe::icontext_receive_buffer_not_empty(QueueProcessor<Event*>* q, ReceiveBufferNotEmptyEvent* e) {
    Socket* s = e->get_socket();
    BasicIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_receive_buffer_not_empty(q, e);
    c->get_connection_manager()->icontext_receive_buffer_not_empty(q, e);
    c->get_congestion_control()->icontext_receive_buffer_not_empty(q, e);
}

void TCPTahoe::icontext_receive_buffer_not_full(QueueProcessor<Event*>* q, ReceiveBufferNotFullEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = (TCPTahoeIContextContainer*) map_.find(s)->second;
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c->get_reliability();

    if (c->get_saved_fin() && rc->get_rcv_wnd() == MAX_TCP_RECEIVE_WINDOW_SIZE) {
        dispatch(c->get_saved_fin());
    }

    c->get_reliability()->icontext_receive_buffer_not_full(q, e);
    c->get_connection_manager()->icontext_receive_buffer_not_full(q, e);
    c->get_congestion_control()->icontext_receive_buffer_not_full(q, e);
}

void TCPTahoe::icontext_send_buffer_not_empty(QueueProcessor<Event*>* q, SendBufferNotEmptyEvent* e) {
    Socket* s = e->get_socket();
    BasicIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_send_buffer_not_empty(q, e);
    c->get_connection_manager()->icontext_send_buffer_not_empty(q, e);
    c->get_congestion_control()->icontext_send_buffer_not_empty(q, e);
}

void TCPTahoe::icontext_send_buffer_not_full(QueueProcessor<Event*>* q, SendBufferNotFullEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = (TCPTahoeIContextContainer*) map_.find(s)->second;

    SendEvent* saved_send_event = c->get_saved_send_event();
    int available_space = saved_send_event ? get_available_room_in_send_buffer(saved_send_event) : -1;

    if (available_space > 0) {
        save_in_buffer_and_send_events(q, saved_send_event, available_space);
        c->set_saved_send_event(0);
    }

    c->get_reliability()->icontext_send_buffer_not_full(q, e);
    c->get_connection_manager()->icontext_send_buffer_not_full(q, e);
    c->get_congestion_control()->icontext_send_buffer_not_full(q, e);
}

bool TCPTahoe::icontext_can_send(Socket* s) {
    BasicIContextContainer* c = map_.find(s)->second;
    return c->get_connection_manager()->icontext_can_send(s);
}

bool TCPTahoe::icontext_can_receive(Socket* s) {
    BasicIContextContainer* c = map_.find(s)->second;
    return c->get_connection_manager()->icontext_can_receive(s);
}

void TCPTahoe::icontext_delete_socket(QueueProcessor<Event*>* q, DeleteSocketEvent* e) {
    Socket* s = e->get_socket();
    BasicIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_delete_socket(q, e);
    c->get_connection_manager()->icontext_delete_socket(q, e);
    c->get_congestion_control()->icontext_delete_socket(q, e);

    map_.erase(s);
    assert(map_.find(s) == map_.end());
}

void TCPTahoe::icontext_set_socket_option(QueueProcessor<Event*>* q, SetSocketOptionEvent* e) {
    Socket* s = e->get_socket();
    BasicIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_set_socket_option(q, e);
    c->get_connection_manager()->icontext_set_socket_option(q, e);
    c->get_congestion_control()->icontext_set_socket_option(q, e);
}

void TCPTahoe::icontext_get_socket_option(QueueProcessor<Event*>* q, GetSocketOptionEvent* e) {
    Socket* s = e->get_socket();
    BasicIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_get_socket_option(q, e);
    c->get_connection_manager()->icontext_get_socket_option(q, e);
    c->get_congestion_control()->icontext_get_socket_option(q, e);
}

int TCPTahoe::get_available_room_in_send_buffer(SendEvent* e) {
    Socket* s = e->get_socket();
    int available = TCP_SOCKET_MAX_BUFFER_SIZE - s->get_send_buffer().size();
    assert(0 <= available);
    return available;
}

void TCPTahoe::save_in_buffer_and_send_events(QueueProcessor<Event*>* q, SendEvent* e, int available_room_in_send_buffer) {
    const char* data = (const char*) e->get_data();

    Socket* s = e->get_socket();
    int num_to_insert = min(available_room_in_send_buffer, (int) e->get_data_length());
    s->get_send_buffer().append(data, num_to_insert);

    ResponseEvent* response_event = ObjectPool<ResponseEvent>::instance().get();
    response_event->set_socket(s);
    response_event->set_message_type(e->get_message_type());
    response_event->set_fd(e->get_fd());
    response_event->set_return_value(num_to_insert);
    response_event->set_errno(0);
    response_event->set_default_length();
    response_event->set_destination(e->get_source());

    dispatch(response_event);
    q->enqueue(new SendBufferNotEmptyEvent(s));
}

bool TCPTahoe::is_valid_sequence_number(TCPTahoeReliabilityContext* rc, TCPPacket* p) {

    // This is the check to ensure we still continue if there is no context yet
    // It is not part of the RFC
    // TODO: this may need to change to something else as we may wrap around
    // I actually cannot remember why this needs to be here -- RB
    if (rc->get_rcv_nxt() == 0) {
        return true;
    }

    // These checks are in reverse order that they are on page 69 of RFC 793
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
    return between_equal(rc->get_snd_una(), p->get_tcp_ack_number(), rc->get_snd_max());
}

void TCPTahoe::send_accept_response(TCPTahoeIContextContainer* c, AcceptEvent* e) {
    ConnectionEstablishedEvent* cee = c->get_back_log()->pop();

    AcceptResponseEvent* response_event = (AcceptResponseEvent*) ObjectPool<ResponseEvent>::instance().get();
    response_event->set_socket(cee->get_socket());
    response_event->set_message_type(e->get_message_type());
    response_event->set_fd(e->get_fd());
    response_event->set_return_value(cee->get_new_socket()->get_socket_id());
    response_event->set_errno(0);
    response_event->set_length(sizeof (struct AcceptResponseMessage));
    response_event->set_destination(e->get_source());
    response_event->set_addr(cee->get_new_socket()->get_remote_address_port()->get_network_struct_ptr(), sizeof (struct sockaddr_in));
    dispatch(response_event);
}