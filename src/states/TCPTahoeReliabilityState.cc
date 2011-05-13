#include "states/TCPTahoeReliabilityState.h"

TCPTahoeReliabilityState::TCPTahoeReliabilityState() {

}

TCPTahoeReliabilityState::~TCPTahoeReliabilityState() {

}

void TCPTahoeReliabilityState::state_send_packet(Context* c, SendPacketEvent* e) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    Socket* s = e->get_socket();
    TCPPacket* p = (TCPPacket*) e->get_packet();

    // Sequence numbers
    if (rc->get_snd_una() == 0) {
        // SYN
        u_int32_t iss = 1;
        p->set_tcp_sequence_number(iss);
        rc->set_snd_una(iss);
        rc->set_snd_nxt(iss + 1);
    } else {
        p->set_tcp_sequence_number(rc->get_snd_nxt());
        u_int32_t len = rc->get_snd_nxt() + p->is_tcp_fin() ? 1 : p->get_data_length_bytes();
        rc->set_snd_nxt(len);
    }

    // ACK number and bit
    p->set_tcp_ack_number(rc->get_rcv_nxt());
    if (p->get_tcp_ack_number()) {
        p->set_tcp_ack(true);
    }

    if (p->get_data_length_bytes() > 0 || p->is_tcp_syn() || p->is_tcp_fin()) {
        start_timer(c, s);
    }

    if (p->is_tcp_syn()) {
        assert(s->get_send_buffer().empty());
        s->get_send_buffer().append(SYN_BYTE);
    } else if (p->is_tcp_fin()) {
        s->get_send_buffer().append(FIN_BYTE);
    }

    p->set_tcp_receive_window_size(rc->get_rcv_wnd());
}

void TCPTahoeReliabilityState::state_timer_fired(Context* c, TimerFiredEvent* e) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    Socket* s = e->get_socket();

    if (rc->get_timeout_event() == e->get_timeout_event()) {

        TCPPacket* p = new TCPPacket();
        p->insert_tcp_header_option(new TCPTimestampOption());

        AddressPort* destination = s->get_remote_address_port();
        AddressPort* source = s->get_local_address_port();

        p->set_ip_destination_address_s(destination->get_address());
        p->set_destination_port(destination->get_port());

        p->set_ip_source_address_s(source->get_address());
        p->set_source_port(source->get_port());

        // Check for SYN or FIN byte in the buffer
        string& send_buffer = s->get_send_buffer();
        assert(!send_buffer.empty());
        bool control_bit = false;

        if (!send_buffer.compare(0, 1, SYN_BYTE.c_str())) {
            p->set_tcp_syn(true);
            control_bit = true;

        } else if (!send_buffer.compare(0, 1, FIN_BYTE.c_str())) {
            p->set_tcp_fin(true);
            control_bit = true;
        }

        if (control_bit) {
            rc->set_snd_nxt(rc->get_snd_una() + 1);
            p->set_data((unsigned char*) "", 0);
        } else {
            string data = send_buffer.substr(0, p->max_data_length());
            if (!data.compare(data.size() - 1, 1, FIN_BYTE.c_str())) {
                data.erase(data.size() - 1, 1);
            }
            rc->set_snd_nxt(rc->get_snd_una() + data.size());
            p->set_data((unsigned char*) data.data(), data.size());
        }

        p->set_tcp_sequence_number(rc->get_snd_una());
        p->set_tcp_ack_number(rc->get_rcv_nxt());

        if (p->get_tcp_ack_number()) {
            p->set_tcp_ack(true);
        }

        ResendPacketEvent* event = new ResendPacketEvent(s, p);
        Dispatcher::instance().enqueue(event);

        rc->set_rto(rc->get_rto() * 2);
        reset_timer(c, s);
    }
}

void TCPTahoeReliabilityState::state_receive_packet(Context* c, NetworkReceivePacketEvent* e) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;
    Socket* s = e->get_socket();
    TCPPacket* p = (TCPPacket*) e->get_packet();

    if(p->is_tcp_ack() && between_equal_right(rc->get_snd_una(), p->get_tcp_sequence_number(), rc->get_snd_nxt())) {
        u_int32_t num_acked = p->get_tcp_ack_number() - rc->get_snd_una();
        rc->set_snd_una(p->get_tcp_ack_number());
        s->get_send_buffer().erase(0, num_acked);

        if(rc->get_snd_nxt() == rc->get_snd_nxt()) {
            // no outstanding data
            cancel_timer(c, s);
        }
        else if (num_acked > 0) {
            // we did ack some data
            reset_timer(c, s);
        }
    }

}

void TCPTahoeReliabilityState::state_receive_buffer_not_empty(Context* c, ReceiveBufferNotEmptyEvent* e) {

}

void TCPTahoeReliabilityState::state_receive(Context* c, ReceiveEvent* e) {

}

void TCPTahoeReliabilityState::start_timer(Context* c, Socket* s) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;

    // only start the timer if it is not already running
    if (!rc->get_timeout_event()) {
        TimeoutEvent* timer = new TimeoutEvent(s, rc->get_rto(), 0);
        rc->set_timeout_event(timer);
        Dispatcher::instance().enqueue(timer);
    }
}

void TCPTahoeReliabilityState::reset_timer(Context* c, Socket* s) {
    cancel_timer(c, s);
    start_timer(c, s);
}

void TCPTahoeReliabilityState::cancel_timer(Context* c, Socket* s) {
    TCPTahoeReliabilityContext* rc = (TCPTahoeReliabilityContext*) c;

    assert(rc->get_timeout_event());
    CancelTimerEvent* event = new CancelTimerEvent(rc->get_timeout_event());
    Dispatcher::instance().enqueue(event);
    rc->set_timeout_event(0);
}
