#include "states/TCPTahoeReliabilityState.h"

TCPTahoeReliabilityState::TCPTahoeReliabilityState() {

}

TCPTahoeReliabilityState::~TCPTahoeReliabilityState() {

}

void TCPTahoeReliabilityState::state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    Socket* s = e->get_socket();
    TCPPacket* p = (TCPPacket*) e->get_packet();
    string& send_buffer = s->get_send_buffer();

    // Sequence numbers
    if (!rc->is_initialized()) {
        // Original SYN
        p->set_tcp_sequence_number(rc->get_iss());
        rc->set_snd_una(rc->get_iss());
        rc->set_snd_nxt(rc->get_iss() + 1);
        rc->set_initialized();
    } else {
        p->set_tcp_sequence_number(rc->get_snd_nxt());
        u_int32_t len = rc->get_snd_nxt() + (p->is_tcp_syn() || p->is_tcp_fin() ? 1 : p->get_data_length_bytes());
        rc->set_snd_nxt(len);
    }

    // ACK number and bit
    // TODO: what about wrap-around and we are ack'ing byte 0?
    p->set_tcp_ack_number(rc->get_rcv_nxt());
    if (p->get_tcp_ack_number()) {
        p->set_tcp_ack(true);
    }

    if (p->get_data_length_bytes() > 0 || p->is_tcp_syn() || p->is_tcp_fin()) {
        start_timer(c, s);
    }


    // if SYN and send buffer doesn't already have the SYN saved (resend)
    if (p->is_tcp_syn() && send_buffer.compare(0, 1, SYN_BYTE.c_str())) {
        assert(s->get_send_buffer().empty());
        send_buffer.append(SYN_BYTE);

    } // if FIN and send buffer doesn't already have the FIN saved (resend)
    else if (p->is_tcp_fin() && (send_buffer.empty() || send_buffer.compare(send_buffer.size() - 1, 1, FIN_BYTE.c_str()))) {
        send_buffer.append(FIN_BYTE);
    }

    p->set_tcp_receive_window_size(rc->get_rcv_wnd());

    TCPTimestampOption* option = (TCPTimestampOption*) p->get_option(TCPOPT_TIMESTAMP);
    assert(option);
    option->set_timestamp();
    if (rc->get_echo_reply()) {
        option->set_echo_reply(rc->get_echo_reply());
    }
}

void TCPTahoeReliabilityState::state_timer_fired(Context* c, QueueProcessor<Event*>* q, TimerFiredEvent* e) {
    cout << "TCPTahoeReliabilityState::state_timer_fired() on socket: " << e->get_socket() << endl;
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    Socket* s = e->get_socket();

    if (rc->get_timeout_event() == e->get_timeout_event()) {
        rc->set_rto(rc->get_rto() * 2);
        resend_data(c, q, s);
    }
}

void TCPTahoeReliabilityState::state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    Socket* s = e->get_socket();
    TCPPacket* p = (TCPPacket*) e->get_packet();

    //        cout << "TCPTahoeReliabilityState::state_receive_packet() on socket: " << s << endl;
    //    cout << p->to_s_format() << endl;
    //    cout << p->to_s() << endl;

    // TODO: we will (for now) blindly update the echo reply here (is this okay?)
    // we can reach this point without validating either the ack or seq number
    TCPTimestampOption* ts = (TCPTimestampOption*) p->get_option(TCPOPT_TIMESTAMP);
    if (ts) {
        rc->set_echo_reply(ts->get_timestamp());
    }

    //    cout << "Received packet TS: " << ts->to_s() << endl;

    if (p->is_tcp_ack() && between_equal_right(rc->get_snd_una(), p->get_tcp_ack_number(), rc->get_snd_nxt())) {
        //        cout << "TCPTahoeReliabilityState::state_receive_packet(), ACK'ing data" << endl;
        u_int32_t num_acked = p->get_tcp_ack_number() - rc->get_snd_una();
        rc->set_snd_una(p->get_tcp_ack_number());
        s->get_send_buffer().erase(0, num_acked);

        // TODO: is this the correct place to update the RTO?
        update_rto(c, ts);

        if (rc->get_snd_nxt() == rc->get_snd_una()) {
            // no outstanding data
            //            cout << "TCPTahoeReliabilityState::state_receive_packet(), canceling timer" << endl;
            cancel_timer(c, s);
            //            cout << "TCPTahoeReliabilityState::state_receive_packet(), timer canceled" << endl;
        } else if (num_acked > 0) {
            // we did ack some data
            reset_timer(c, s);
        }
    } else if (p->is_tcp_ack() && less_than(rc->get_snd_nxt(), p->get_tcp_ack_number())) {
        create_and_dispatch_ack(q, s);
        return;
    } else if (p->is_tcp_ack() &&
            between_equal_left(p->get_tcp_ack_number(), rc->get_snd_una(), rc->get_snd_nxt())) {

        if (rc->get_duplicate_ack_number() == p->get_tcp_ack_number()) {
            rc->set_duplicates(rc->get_duplicates() + 1);
        } else {
            rc->set_duplicate_ack_number(p->get_tcp_ack_number());
            rc->set_duplicates(1);
        }
        if (rc->get_duplicates() == 3) {
            rc->set_duplicates(0);
            cout << "Three duplicate acks, resending data" << endl;

            // I read the following three lines of comments from inet/src/transport/tcp/flavours/TCPTahoe.cc
            // Do not restart REXMIT timer.
            // Note: Restart of REXMIT timer on retransmission is not part of RFC 2581, however optional in RFC 3517 if sent during recovery.
            // Resetting the REXMIT timer is discussed in RFC 2582/3782 (NewReno) and RFC 2988.
            resend_data(c, q, s);
        }
    }


    if (p->is_tcp_syn() || p->is_tcp_fin()) {
        rc->set_rcv_nxt(p->get_tcp_sequence_number() + 1);
        if (p->is_tcp_syn()) {
            rc->get_receive_window().set_first_sequence_number(rc->get_rcv_nxt());
        }
    } else if (p->get_data_length_bytes() > 0) {

        int num_inserted = rc->get_receive_window().insert(p);
        if (rc->get_rcv_wnd() - num_inserted >= 0) {
            //        cout << "TCPTahoeReliabilityState::state_receive_packet(), DATA found" << endl;
            // save data

            //        cout << "TCPTahoeReliabilityState::state_receive_packet(), num put in receive window: " << num_inserted << endl;
            rc->set_rcv_wnd(rc->get_rcv_wnd() - num_inserted);

            //        cout << "TCPTahoeReliabilityState::state_receive_packet(), receive window first byte: " << rc->get_receive_window().get_first_sequence_number() << endl;
            //        cout << "TCPTahoeReliabilityState::state_receive_packet(), RCV.NXT: " << rc->get_rcv_nxt() << endl;

            string& receive_buffer = s->get_receive_buffer();
            u_int32_t before_rcv_buffer_size = receive_buffer.size();
            //        cout << "TCPTahoeReliabilityState::state_receive_packet(), Receive receive buffer before size: " << before_rcv_buffer_size << endl;



            rc->get_receive_window().get_continuous_data(rc->get_rcv_nxt(), receive_buffer);
            u_int32_t after_receive_buffer_size = receive_buffer.size();
            //        cout << "TCPTahoeReliabilityState::state_receive_packet(), Arter receive buffer before size: " << after_receive_buffer_size << endl;
            u_int32_t amount_put_in_receive_buffer = after_receive_buffer_size - before_rcv_buffer_size;
            //        cout << "TCPTahoeReliabilityState::state_receive_packet(), Amount put in receive buffer: " << amount_put_in_receive_buffer << endl;
            assert(amount_put_in_receive_buffer >= 0);

            if (amount_put_in_receive_buffer > 0) {
                rc->set_rcv_nxt(rc->get_rcv_nxt() + amount_put_in_receive_buffer);
                q->enqueue(new ReceiveBufferNotEmptyEvent(s));
            }
        }
        else {
            // I don't think we should get here.
            // I ran some tests with the assert on and it never asserted.
            // But just in case...
            //assert(false);
            rc->get_receive_window().remove(p);
        }

        create_and_dispatch_ack(q, s);
    }
}

void TCPTahoeReliabilityState::state_receive_buffer_not_empty(Context* c, QueueProcessor<Event*>* q, ReceiveBufferNotEmptyEvent* e) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    Socket* s = e->get_socket();

    if (rc->get_receive_event() && !s->get_receive_buffer().empty()) {
        create_and_dispatch_received_data(c, q, rc->get_receive_event());
        rc->set_receive_event(0);
    }
}

void TCPTahoeReliabilityState::state_receive(Context* c, QueueProcessor<Event*>* q, ReceiveEvent* e) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    Socket* s = e->get_socket();

    if (!s->get_receive_buffer().empty()) {
        create_and_dispatch_received_data(c, q, e);
    } else {
        assert(!rc->get_receive_event());
        rc->set_receive_event(e);
    }
}

void TCPTahoeReliabilityState::create_and_dispatch_ack(QueueProcessor<Event*>* q, Socket* s) {
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
    //    cout << "TCPTahoeReliabilityState::start_timer() on socket: " << s << endl;
    // only start the timer if it is not already running
    if (!rc->get_timeout_event()) {
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
    //    cout << "TCPTahoeReliabilityState::cancel_timer() on socket: " << s << endl;
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;

    assert(rc->get_timeout_event());
    //    cout << "TCPTahoeReliabilityState::cancel_timer(): " << rc->get_timeout_event() << endl;
    CancelTimerEvent* event = new CancelTimerEvent(rc->get_timeout_event());
    Dispatcher::instance().enqueue(event);
    rc->set_timeout_event(0);
}

void TCPTahoeReliabilityState::resend_data(Context* c, QueueProcessor<Event*>* q, Socket* s) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    rc->set_snd_nxt(rc->get_snd_una());
    q->enqueue(new ResendPacketEvent(s));
}

void TCPTahoeReliabilityState::create_and_dispatch_received_data(Context* c, QueueProcessor<Event*>* q, ReceiveEvent* e) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    Socket* s = e->get_socket();
    int buffer_size = e->get_receive_buffer_size();

    // TODO: change this to use string::data() so we avoid the copy in substr
    string data = s->get_receive_buffer().substr(0, buffer_size);
    int length = data.size();
    s->get_receive_buffer().erase(0, length);

    rc->set_rcv_wnd(rc->get_rcv_wnd() + length);

    ResponseEvent* response = new ResponseEvent(s, e->get_name(), e->get_map()[FILE_STRING]);
    response->put(BUFFER_STRING, data);
    response->put(ADDRESS_STRING, s->get_remote_address_port()->get_address());
    response->put(PORT_STRING, Utils::itoa(s->get_remote_address_port()->get_port()));
    response->put(RETURN_VALUE_STRING, Utils::itoa(data.size()));
    response->put(ERRNO, Utils::itoa(0));

    Dispatcher::instance().enqueue(response);
    q->enqueue(new ReceiveBufferNotFullEvent(s));
}

void TCPTahoeReliabilityState::update_rto(Context* c, TCPTimestampOption* ts) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;

    double rtt = Utils::get_current_time_microseconds_32() - ts->get_echo_reply();
    rtt /= MICROSECONDS_IN_SECONDS;

    //        cout << "RTT: " << rtt << endl;

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

    //        cout << "Updated RTO: " << rc->get_rto() << endl;
}