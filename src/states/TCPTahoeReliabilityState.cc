#include "states/TCPTahoeReliabilityState.h"


TCPTahoeReliabilityState::TCPTahoeReliabilityState() {

}

TCPTahoeReliabilityState::~TCPTahoeReliabilityState() {

}

void TCPTahoeReliabilityState::state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e) {
    TCPPacket* p = (TCPPacket*) e->get_packet();

    set_sequence_number_and_update_window_variables(c, p);
    set_ack_number_and_bit(c, p);
    check_and_start_timer(c, e);
    append_control_bytes_to_send_buffer(e);
    insert_timestamp(c, p);
    set_and_save_receive_window_size(c, p);
}

void TCPTahoeReliabilityState::state_timer_fired(Context* c, QueueProcessor<Event*>* q, TimerFiredEvent* e) {
//    cout << "TCPTahoeReliabilityState::state_timer_fired() on socket: " << e->get_socket() << endl;
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    Socket* s = e->get_socket();

    if (rc->get_timeout_event() == e->get_timeout_event()) {
        rc->set_timeout_event(0);
        rc->set_rto(rc->get_rto() * 2);
        resend_data(c, q, s, TIMEOUT);
    }
}

void TCPTahoeReliabilityState::state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    TCPPacket* p = (TCPPacket*) e->get_packet();

    update_echo_reply(c, p);
    if(!handle_ack(c, q,e )) return;
    handle_control_bits_and_data(c, q, e);
}

void TCPTahoeReliabilityState::state_receive_buffer_not_empty(Context* c, QueueProcessor<Event*>* q, ReceiveBufferNotEmptyEvent* e) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    Socket* s = e->get_socket();

//    cout << "TCPTahoeReliabilityState::state_receive_buffer_not_empty()" << endl;

    if (rc->get_receive_event() && !s->get_receive_buffer().empty()) {
//        cout << "TCPTahoeReliabilityState::state_receive_buffer_not_empty(), receive event exists" << endl;
        create_and_dispatch_received_data(c, q, rc->get_receive_event());
        rc->set_receive_event(0);
    }
}

void TCPTahoeReliabilityState::state_receive(Context* c, QueueProcessor<Event*>* q, ReceiveEvent* e) {
    cout << "TCPTahoeReliabilityState::state_receive()" << endl;
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    Socket* s = e->get_socket();

    if (!s->get_receive_buffer().empty()) {
        cout << "Receive buffer not empty" << endl;
        create_and_dispatch_received_data(c, q, e);
    } else {
        cout << "Receive buffer empty" << endl;
        assert(!rc->get_receive_event());
        rc->set_receive_event(e);
    }
}

void TCPTahoeReliabilityState::create_and_dispatch_ack(Context* c, QueueProcessor<Event*>* q, Event* e) {
    Socket* s = e->get_socket();
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
}

void TCPTahoeReliabilityState::start_timer(Context* c, Socket* s) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
        
    // only start the timer if it is not already running
    if (!rc->get_timeout_event()) {
//        cout << "TCPTahoeReliabilityState::start_timer() on socket: " << s << endl;
        double seconds;
        long int nanoseconds = modf(rc->get_rto(), &seconds) * NANOSECONDS_IN_SECONDS;
        TimeoutEvent* timer = new TimeoutEvent(s, seconds, nanoseconds);
        rc->set_timeout_event(timer);
        Dispatcher::instance().enqueue(timer);
    }
}

void TCPTahoeReliabilityState::reset_timer(Context* c, Socket* s) {
    //    cout << "TCPTahoeReliabilityState::reset_timer() on socket: " << s << endl;
    cancel_timer(c, s);
    start_timer(c, s);
}

void TCPTahoeReliabilityState::cancel_timer(Context* c, Socket* s) {
//        cout << "TCPTahoeReliabilityState::cancel_timer() on socket: " << s << endl;
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;

    assert(rc->get_timeout_event());
    //    cout << "TCPTahoeReliabilityState::cancel_timer(): " << rc->get_timeout_event() << endl;
    CancelTimerEvent* event = new CancelTimerEvent(rc->get_timeout_event());
    Dispatcher::instance().enqueue(event);
    rc->set_timeout_event(0);
}

void TCPTahoeReliabilityState::resend_data(Context* c, QueueProcessor<Event*>* q, Socket* s, ResendReason reason) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    rc->set_snd_nxt(rc->get_snd_una());
    q->enqueue(new ResendPacketEvent(s, reason));
}

void TCPTahoeReliabilityState::create_and_dispatch_received_data(Context* c, QueueProcessor<Event*>* q, ReceiveEvent* e) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    Socket* s = e->get_socket();
    size_t buffer_size = e->get_data_length();

    RecvFromResponseEvent* response = (RecvFromResponseEvent*) ObjectPool<ResponseEvent>::instance().get();
    response->set_socket(s);
    response->set_message_type(e->get_message_type());
    response->set_fd(e->get_fd());
    response->set_destination(e->get_source());
    response->set_addr(s->get_remote_address_port()->get_network_struct_ptr(), sizeof(struct sockaddr_in));
    // done in set length
    //response->set_return_value(length);
    response->set_errno(0);

    int length = min(s->get_receive_buffer().size() - rc->get_receive_index(), buffer_size);
    response->set_return_buffer((unsigned char*) s->get_receive_buffer().data() + rc->get_receive_index(), length);

    rc->set_receive_index(rc->get_receive_index() + length);

    if(rc->get_rcv_wnd() + length >= 2 * rc->get_rcv_wnd()) {
        // send notification packet because our receive window has at least doubled in size
        // RFC 2581 Section 4.2 and RFC 813 (I think?)
        create_and_dispatch_ack(c, q, e);
    }

    rc->set_rcv_wnd(rc->get_rcv_wnd() + length);
    if(rc->get_receive_index() >= s->get_receive_buffer().size()) {
        s->get_receive_buffer().clear();
        rc->set_receive_index(0);
    }

    Dispatcher::instance().enqueue(response);
    q->enqueue(new ReceiveBufferNotFullEvent(s));
}

void TCPTahoeReliabilityState::update_rto(Context* c, TCPTimestampOption* ts) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;

    double rtt = Utils::get_current_time_microseconds_32() - ts->get_echo_reply();
    rtt /= MICROSECONDS_IN_SECONDS;

    // From here all arithmetic is done in seconds
    // first RTT calculation
    if (rc->get_srtt() < 0) {
        rc->set_srtt(rtt);
        rc->set_rttvar(rtt / 2);
    } else {
        rc->set_rttvar(((1 - BETA) * rc->get_rttvar()) + (BETA * abs(rc->get_srtt() - rtt)));
        rc->set_srtt(((1 - ALPHA) * rc->get_srtt()) + (ALPHA * rtt));
    }

    rc->set_rto(max(MIN_RTO, rc->get_srtt() + max(G, K * rc->get_rttvar())));
}


// <editor-fold defaultstate="collapsed" desc="state_send_packet() helper functions">
void TCPTahoeReliabilityState::set_sequence_number_and_update_window_variables(Context* c, TCPPacket* p) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;

    if (!rc->is_initialized()) {
        // Original SYN
        p->set_tcp_sequence_number(rc->get_iss());
        rc->set_snd_una(rc->get_iss());
        rc->set_snd_nxt(rc->get_iss() + 1);
        rc->set_initialized();
    } else {
        p->set_tcp_sequence_number(rc->get_snd_nxt());
        u_int32_t len = (p->is_tcp_syn() || p->is_tcp_fin() ? 1 : p->get_data_length_bytes());
        rc->set_snd_nxt(rc->get_snd_nxt() + len);
    }
    
}

void TCPTahoeReliabilityState::set_ack_number_and_bit(Context* c, TCPPacket* p) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    // TODO: what about wrap-around and we are ack'ing byte 0?
    p->set_tcp_ack_number(rc->get_rcv_nxt());
    if (p->get_tcp_ack_number()) {
        p->set_tcp_ack(true);
    }
}

void TCPTahoeReliabilityState::check_and_start_timer(Context*c, SendPacketEvent* e) {
    TCPPacket* p = (TCPPacket*) e->get_packet();
    if (p->get_data_length_bytes() > 0 || p->is_tcp_syn() || p->is_tcp_fin()) {
        start_timer(c, e->get_socket());
    }
}

void TCPTahoeReliabilityState::append_control_bytes_to_send_buffer(SendPacketEvent* e) {
    TCPPacket* p = (TCPPacket*)e->get_packet();
    gcstring& send_buffer = e->get_socket()->get_send_buffer();

    // if SYN and send buffer doesn't already have the SYN saved (resend)
    if (p->is_tcp_syn() && send_buffer.compare(0, 1, SYN_BYTE.c_str())) {
        assert(send_buffer.empty());
        send_buffer.append(SYN_BYTE);

    }// if FIN and send buffer doesn't already have the FIN saved (resend)
    else if (p->is_tcp_fin() && (send_buffer.empty() || send_buffer.compare(send_buffer.size() - 1, 1, FIN_BYTE.c_str()))) {
        send_buffer.append(FIN_BYTE);
    }
}

void TCPTahoeReliabilityState::insert_timestamp(Context* c, TCPPacket* p) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;

    TCPTimestampOption* option = (TCPTimestampOption*) p->get_option(TCPOPT_TIMESTAMP);
    assert(option);
    option->set_timestamp();
    if (rc->get_echo_reply()) {
        option->set_echo_reply(rc->get_echo_reply());
    }
}

void TCPTahoeReliabilityState::set_and_save_receive_window_size(Context* c, TCPPacket* p) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    p->set_tcp_receive_window_size(rc->get_rcv_wnd());
} // </editor-fold>

// <editor-fold defaultstate="collapsed" desc="state_receive_packet() helper functions">

void TCPTahoeReliabilityState::update_echo_reply(Context* c, TCPPacket* p) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;

    // TODO: we will (for now) blindly update the echo reply here (is this okay?)
    // we can reach this point without validating either the ack or seq number
    TCPTimestampOption* ts = (TCPTimestampOption*) p->get_option(TCPOPT_TIMESTAMP);
    if (ts) {
        rc->set_echo_reply(ts->get_timestamp());
    }
}

bool TCPTahoeReliabilityState::handle_ack(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    TCPPacket* p = (TCPPacket*) e->get_packet();

    if (!p->is_tcp_ack()) {
        return true;
    }

    if (between_equal_right(rc->get_snd_una(), p->get_tcp_ack_number(), rc->get_snd_max())) {
        handle_valid_ack(c, q, e);
    } else if (less_than(rc->get_snd_max(), p->get_tcp_ack_number())) {
        // invalid ack
        create_and_dispatch_ack(c, q, e);
        return false;
    } else if (between_equal_left(p->get_tcp_ack_number(), rc->get_snd_una(), rc->get_snd_nxt())) {
        handle_duplicate_ack(c, q, e);
    }
    return true;
}

void TCPTahoeReliabilityState::handle_valid_ack(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent * e) {
    //    cout << "TCPTahoeReliabilityState::handle_valid_ack() Socket: " << e->get_socket() << endl;
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    TCPPacket* p = (TCPPacket*) e->get_packet();
    Socket* s = e->get_socket();


    u_int32_t num_acked = p->get_tcp_ack_number() - rc->get_snd_una();
    rc->set_snd_una(p->get_tcp_ack_number());

    // In case we get an ack for something later than snd.nxt
    // (we dropped a packet but subsequent packets got through and we received a cumuliative ack)
    if (less_than(rc->get_snd_nxt(), rc->get_snd_una())) {
        rc->set_snd_nxt(rc->get_snd_una());
    }

    // TODO: Replace this erase()
    s->get_send_buffer().erase(0, num_acked);

    // TODO: this may need to move if we decide that we want to open up the send buffer on things other than data acks
    q->enqueue(new SendBufferNotFullEvent(s));


    // TODO: is this the correct place to update the RTO?
    TCPTimestampOption* ts = (TCPTimestampOption*) p->get_option(TCPOPT_TIMESTAMP);
    update_rto(c, ts);

    if (rc->get_snd_nxt() == rc->get_snd_una()) {
        // no outstanding data
        cancel_timer(c, s);
    } else if (num_acked > 0) {
        // we did ack some data
        reset_timer(c, s);
    }
}

void TCPTahoeReliabilityState::handle_duplicate_ack(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent * e) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    TCPPacket* p = (TCPPacket*) e->get_packet();

    if (rc->get_duplicate_ack_number() == p->get_tcp_ack_number()) {
        rc->set_duplicates(rc->get_duplicates() + 1);
    } else {
        rc->set_duplicate_ack_number(p->get_tcp_ack_number());
        rc->set_duplicates(1);
    }
    if (rc->get_duplicates() == 3) {
        rc->set_duplicates(0);
        //        cout << "Three duplicate acks, resending data" << endl;

        // I read the following three lines of comments from inet/src/transport/tcp/flavours/TCPTahoe.cc
        // Do not restart REXMIT timer.
        // Note: Restart of REXMIT timer on retransmission is not part of RFC 2581, however optional in RFC 3517 if sent during recovery.
        // Resetting the REXMIT timer is discussed in RFC 2582/3782 (NewReno) and RFC 2988.
        resend_data(c, q, e->get_socket(), TIMEOUT);
    }
}

void TCPTahoeReliabilityState::handle_control_bits_and_data(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent * e) {
    TCPPacket* p = (TCPPacket*) e->get_packet();

    if (p->is_tcp_syn() || p->is_tcp_fin()) {
        handle_control_bits(c, q, e);
    } else if (p->get_data_length_bytes() > 0) {
        handle_data(c, q, e);
    }
}

void TCPTahoeReliabilityState::handle_control_bits(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent * e) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    TCPPacket* p = (TCPPacket*) e->get_packet();

    rc->set_rcv_nxt(p->get_tcp_sequence_number() + 1);
    if (p->is_tcp_syn()) {
        rc->get_receive_window().set_first_sequence_number(rc->get_rcv_nxt());
    }
}

void TCPTahoeReliabilityState::handle_data(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent * e) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    TCPPacket* p = (TCPPacket*) e->get_packet();
    Socket* s = e->get_socket();

    int num_inserted = rc->get_receive_window().insert(p);
    if (rc->get_rcv_wnd() - num_inserted >= 0) {

        rc->set_rcv_wnd(rc->get_rcv_wnd() - num_inserted);
        //        cout << "TCPTahoeReliabilityState::handle_data(), receive window size: " << rc->get_rcv_wnd() << endl;
        gcstring& receive_buffer = s->get_receive_buffer();
        u_int32_t before_rcv_buffer_size = receive_buffer.size();
        rc->get_receive_window().get_continuous_data(rc->get_rcv_nxt(), receive_buffer);
        u_int32_t after_receive_buffer_size = receive_buffer.size();
        u_int32_t amount_put_in_receive_buffer = after_receive_buffer_size - before_rcv_buffer_size;
        assert(amount_put_in_receive_buffer >= 0);

        if (amount_put_in_receive_buffer > 0) {
            rc->set_rcv_nxt(rc->get_rcv_nxt() + amount_put_in_receive_buffer);
            q->enqueue(new ReceiveBufferNotEmptyEvent(s));
        }
    } else {
        // I don't think we should get here.
        // I ran some tests with the assert on and it never asserted.
        // But just in case...
        assert(false);
        rc->get_receive_window().remove(p);
    }

    create_and_dispatch_ack(c, q, e);
}
// </editor-fold>
