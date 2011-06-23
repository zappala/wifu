/* 
 * File:   TCPTahoeBaseCongestionControl.cc
 * Author: rbuck
 * 
 * Created on May 23, 2011, 2:59 PM
 */

#include "states/TCPTahoeBaseCongestionControl.h"
// must be here to avoid circular dependencies
#include "contexts/TCPTahoeCongestionControlContext.h"

TCPTahoeBaseCongestionControl::TCPTahoeBaseCongestionControl() : State() {
}

TCPTahoeBaseCongestionControl::TCPTahoeBaseCongestionControl(const TCPTahoeBaseCongestionControl& orig) : State() {
}

TCPTahoeBaseCongestionControl::~TCPTahoeBaseCongestionControl() {
}

void TCPTahoeBaseCongestionControl::state_timer_fired(Context* c, QueueProcessor<Event*>* q, TimerFiredEvent* e) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;

    if (ccc->get_probe_timer() && e->get_timeout_event() == ccc->get_probe_timer()) {
        // this is our probe timer
//        cout << "TCPTahoeBaseCongestionControl::state_timer_fired() Timer Fired" << endl;

        // restart the probe timer
        ccc->set_probe_timer_duration(ccc->get_probe_timer_duration() * 2);
        TimeoutEvent* event = new TimeoutEvent(e->get_socket(), ccc->get_probe_timer_duration(), 0);
        ccc->set_probe_timer(event);
        Dispatcher::instance().enqueue(event);

        send_one_packet(c, q, e, true);
    }
}

void TCPTahoeBaseCongestionControl::state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
    TCPPacket* p = (TCPPacket*) e->get_packet();

    if (p->is_tcp_syn()) {
        ccc->set_snd_una(ccc->get_iss());
        ccc->set_snd_nxt(ccc->get_iss() + 1);
    } else if (p->is_tcp_fin()) {
        ccc->set_snd_nxt(ccc->get_snd_nxt() + 1);
    }

    if (!ccc->is_data_sent() && p->get_data_length_bytes() > 0) {
        ccc->set_data_sent(true);
    }

    // we will set snd.nxt for data when we originally send data
}

void TCPTahoeBaseCongestionControl::state_resend_packet(Context* c, QueueProcessor<Event*>* q, ResendPacketEvent* e) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;

    resend(c, q, e);
    ccc->set_snd_nxt(ccc->get_snd_una());
    resend_data(c, q, e);
}

void TCPTahoeBaseCongestionControl::state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
//    cout << "TCPTahoeBaseCongestionControl::state_receive_packet()" << endl;
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
    TCPPacket* p = (TCPPacket*) e->get_packet();

    if (p->is_tcp_ack() && between_equal_right(ccc->get_snd_una(), p->get_tcp_ack_number(), ccc->get_snd_max())) {
        ccc->set_snd_una(p->get_tcp_ack_number());

        // In case we get an ack for something later than snd.nxt
        // (we dropped a packet but subsequent packets got through and we received a cumuliative ack)
        if (less_than(ccc->get_snd_nxt(), ccc->get_snd_una())) {
            ccc->set_snd_nxt(ccc->get_snd_una());
        }

        // update send window (RFC p. 72)
        if (less_than(ccc->get_snd_wnd1(), p->get_tcp_sequence_number()) ||
                (ccc->get_snd_wnd1() == p->get_tcp_sequence_number() &&
                less_than_or_equal(ccc->get_snd_wnd2(), p->get_tcp_ack_number()))) {

            ccc->set_snd_wnd(p->get_tcp_receive_window_size());
            ccc->set_snd_wnd1(p->get_tcp_sequence_number());
            ccc->set_snd_wnd2(p->get_tcp_ack_number());
        }

        if (ccc->is_data_sent()) {
            set_cwnd(c, q, e);
        }

//        cout << "TCPTahoeBaseCongestionControl::state_receive_packet(), num outstanding: " << ccc->get_num_outstanding() << endl;
//        cout << "TCPTahoeBaseCongestionControl::state_receive_packet(), max allowed to send: " << ccc->get_max_allowed_to_send() << endl;

        // check to see if there is room to send data
        assert(ccc->get_num_outstanding() <= ccc->get_max_allowed_to_send());
        if (ccc->get_num_outstanding() == ccc->get_max_allowed_to_send()) {
            // set timer for probe packet
            if (!ccc->get_probe_timer()) {
                TimeoutEvent* timer = new TimeoutEvent(e->get_socket(), ccc->get_probe_timer_duration(), 0);
                ccc->set_probe_timer(timer);
                Dispatcher::instance().enqueue(timer);
            }
        } else {
            // cancel timer
            if (ccc->get_probe_timer()) {

                ccc->set_probe_timer_duration(INITIAL_PROBE_TIMEOUT_DURATION);
                CancelTimerEvent* cancel_timer = new CancelTimerEvent(ccc->get_probe_timer());
                ccc->set_probe_timer(0);
                Dispatcher::instance().enqueue(cancel_timer);
            }
//            cout << "TCPTahoeBaseCongestionControl::state_receive_packet(), sending packets" << endl;
            send_packets(c, q, e);
        }
    }
}

void TCPTahoeBaseCongestionControl::state_send_buffer_not_empty(Context* c, QueueProcessor<Event*>* q, SendBufferNotEmptyEvent* e) {
//    cout << "TCPTahoeBaseCongestionControl::state_send_buffer_not_empty(), sending packets" << endl;
    send_packets(c, q, e);
}

void TCPTahoeBaseCongestionControl::set_cwnd(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    // do nothing here
    // overriding state should set this congestion window according to whatever algorithm it represents
}

void TCPTahoeBaseCongestionControl::resend(Context* c, QueueProcessor<Event*>* q, ResendPacketEvent* e) {
    // do nothing here
    // overriding state should respond to a resend event
}

void TCPTahoeBaseCongestionControl::send_packets(Context* c, QueueProcessor<Event*>* q, Event* e) {
//    cout << "TCPTahoeBaseCongestionControl::send_packets()" << endl;
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
    Socket* s = e->get_socket();
    string& send_buffer = s->get_send_buffer();

    assert(ccc->get_num_outstanding() <= ccc->get_max_allowed_to_send());

    while ((int) send_buffer.size() - (int) ccc->get_num_outstanding() > 0 && ccc->get_num_outstanding() < ccc->get_max_allowed_to_send()) {

        send_one_packet(c, q, e);
    }
}

void TCPTahoeBaseCongestionControl::send_one_packet(Context* c, QueueProcessor<Event*>* q, Event* e, bool ignore_window) {

//    cout << "TCPTahoeBaseCongestionControl::send_one_packet()" << endl;
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
    Socket* s = e->get_socket();

    string& send_buffer = s->get_send_buffer();

    // Represents the first byte in the send buffer which has "never" been sent before.
    // We actually may have sent data already but if SND.UNA changes due to a drop we will treat it as if we never sent it.
    int index = ccc->get_num_outstanding();

    TCPPacket* p = new TCPPacket();
    p->insert_tcp_header_option(new TCPTimestampOption());

    int data_length = get_send_data_length(c, e, p, ignore_window);

    assert(send_buffer.size() > 0);

    const char* data = (send_buffer.data() + index);

    ccc->set_snd_nxt(ccc->get_snd_nxt() + data_length);

    AddressPort* destination = s->get_remote_address_port();
    AddressPort* source = s->get_local_address_port();

    p->set_ip_destination_address_s(destination->get_address());
    p->set_destination_port(destination->get_port());

    p->set_ip_source_address_s(source->get_address());
    p->set_source_port(source->get_port());

    p->set_data((unsigned char*) data, data_length);

    assert(p->get_data_length_bytes() > 0);

    q->enqueue(new SendPacketEvent(s, p));
    // TODO: I moved this to relability when we get an ack
    //    q->enqueue(new SendBufferNotFullEvent(s));
}

void TCPTahoeBaseCongestionControl::resend_data(Context* c, QueueProcessor<Event*>* q, Event* e) {
//    cout << "TCPTahoeBaseCongestionControl::resend_data()" << endl;
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
    Socket* s = e->get_socket();

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
        //                cout << "Control bit set, setting snd_nxt to snd.una + 1" << endl;
        p->set_data((unsigned char*) "", 0);
    } else {
        // TODO: change this to use the string::data() method instead of substr() so we can avoid the copy
        int length = get_resend_data_length(c, e, p);
        if (!send_buffer.compare(send_buffer.size() - 1, 1, FIN_BYTE.c_str())) {

            length -= 1;
        }
        p->set_data((unsigned char*) send_buffer.data(), length);
        assert(p->get_data_length_bytes() > 0);
    }

    u_int32_t length = (p->is_tcp_syn() || p->is_tcp_fin()) ? 1 : p->get_data_length_bytes();
    ccc->set_snd_nxt(ccc->get_snd_una() + length);

    SendPacketEvent* event = new SendPacketEvent(s, p);
    q->enqueue(event);
}

// TODO: these two data lenght calculators could (and should) be refactored together at some point

int TCPTahoeBaseCongestionControl::get_send_data_length(Context* c, Event* e, WiFuPacket* p, bool ignore_window) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
    string& send_buffer = e->get_socket()->get_send_buffer();

    int num_unsent = (int) send_buffer.size() - (int) ccc->get_num_outstanding();

    // we do not want to make a packet larger than the window size
    int data_length = min(min(num_unsent, (int) p->max_data_length()), MAX_TCP_RECEIVE_WINDOW_SIZE);
    if (!ignore_window) {
        int available_window_space = (int) ccc->get_max_allowed_to_send() - (int) ccc->get_num_outstanding();
        data_length = min(data_length, available_window_space);
    }

    assert(data_length > 0);
    return data_length;
}

int TCPTahoeBaseCongestionControl::get_resend_data_length(Context* c, Event* e, WiFuPacket* p) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
    string& send_buffer = e->get_socket()->get_send_buffer();

    // SND.NXT should have previously been set back to SND.UNA
    int num_unsent = (int) send_buffer.size() - (int) ccc->get_num_outstanding();

    // we do not want to make a packet larger than the window size
    int data_length = min(min(num_unsent, (int) p->max_data_length()), min((int) ccc->get_max_allowed_to_send(), MAX_TCP_RECEIVE_WINDOW_SIZE));
    assert(data_length > 0);
    return data_length;
}

