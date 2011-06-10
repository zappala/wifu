/* 
 * File:   DummyCongestionController.cc
 * Author: rbuck
 * 
 * Created on May 23, 2011, 2:59 PM
 */

#include "states/DummyCongestionController.h"

DummyCongestionController::DummyCongestionController() : State() {
}

DummyCongestionController::DummyCongestionController(const DummyCongestionController& orig) : State() {
}

DummyCongestionController::~DummyCongestionController() {
}

void DummyCongestionController::state_timer_fired(Context* c, QueueProcessor<Event*>* q, TimerFiredEvent* e) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;

    if (ccc->get_probe_timer() && e->get_timeout_event() == ccc->get_probe_timer()) {
        // this is our probe timer
        cout << "DummyCongestionController::state_timer_fired() Timer Fired" << endl;

        // restart the probe timer
        ccc->set_probe_timer_duration(ccc->get_probe_timer_duration() * 2);
        TimeoutEvent* event = new TimeoutEvent(e->get_socket(), ccc->get_probe_timer_duration(), 0);
        ccc->set_probe_timer(event);
        Dispatcher::instance().enqueue(event);

        send_one_packet(c, q, e, true);
    }
}

void DummyCongestionController::state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
    TCPPacket* p = (TCPPacket*) e->get_packet();

    if (p->is_tcp_syn()) {
        ccc->set_snd_una(ccc->get_iss());
        ccc->set_snd_nxt(ccc->get_iss() + 1);
    } else if (p->is_tcp_fin()) {
        ccc->set_snd_nxt(ccc->get_snd_nxt() + 1);
    }
    // we will set snd.nxt for data when we originally send data
}

void DummyCongestionController::state_resend_packet(Context* c, QueueProcessor<Event*>* q, ResendPacketEvent* e) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
    ccc->set_snd_nxt(ccc->get_snd_una());
    resend_data(c, q, e);
    // TODO: resize the window?
}

void DummyCongestionController::state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    //    cout << "DummyCongestionContrller::state_receive_packet()" << endl;
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
    TCPPacket* p = (TCPPacket*) e->get_packet();

    if (p->is_tcp_ack() && between_equal_right(ccc->get_snd_una(), p->get_tcp_ack_number(), ccc->get_snd_nxt())) {
        ccc->set_snd_una(p->get_tcp_ack_number());

        // update send window (RFC p. 72)
        if (less_than(ccc->get_snd_wnd1(), p->get_tcp_sequence_number()) ||
                (ccc->get_snd_wnd1() == p->get_tcp_sequence_number() &&
                less_than_or_equal(ccc->get_snd_wnd2(), p->get_tcp_ack_number()))) {

            ccc->set_snd_wnd(p->get_tcp_receive_window_size());
            ccc->set_snd_wnd1(p->get_tcp_sequence_number());
            ccc->set_snd_wnd2(p->get_tcp_ack_number());
        }


        // check to see if there is room in the send window to send data
        if (ccc->get_num_outstanding() == ccc->get_snd_wnd()) {
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
            send_packets(c, q, e);
        }
    }
}

void DummyCongestionController::state_send_buffer_not_empty(Context* c, QueueProcessor<Event*>* q, SendBufferNotEmptyEvent* e) {
    send_packets(c, q, e);
}

void DummyCongestionController::send_packets(Context* c, QueueProcessor<Event*>* q, Event* e) {
    cout << "DummyCongestionController::send_packets()" << endl;
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
    Socket* s = e->get_socket();
    string& send_buffer = s->get_send_buffer();

    assert(ccc->get_num_outstanding() <= ccc->get_snd_wnd());

    while ((int) send_buffer.size() - (int) ccc->get_num_outstanding() > 0 && ccc->get_num_outstanding() < ccc->get_snd_wnd()) {
        send_one_packet(c, q, e);
    }
}

void DummyCongestionController::send_one_packet(Context* c, QueueProcessor<Event*>* q, Event* e, bool ignore_window) {
    cout << "DummyCongestionController::send_one_packet()" << endl;
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
    q->enqueue(new SendBufferNotFullEvent(s));
}

void DummyCongestionController::resend_data(Context* c, QueueProcessor<Event*>* q, Event* e) {
    cout << "DummyCongestionController::resend_data()" << endl;
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
        if(!send_buffer.compare(send_buffer.size() - 1, 1, FIN_BYTE.c_str())) {
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
int DummyCongestionController::get_send_data_length(Context* c, Event* e, WiFuPacket* p, bool ignore_window) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
    string& send_buffer = e->get_socket()->get_send_buffer();

    int num_unsent = (int) send_buffer.size() - (int) ccc->get_num_outstanding();

    // we do not want to make a packet larger than the window size
    int data_length = min(min(num_unsent, (int) p->max_data_length()), MAX_TCP_RECEIVE_WINDOW_SIZE);
    if (!ignore_window) {
        int available_window_space = (int) ccc->get_snd_wnd() - (int) ccc->get_num_outstanding();
        data_length = min(data_length, available_window_space);
    }

    assert(data_length > 0);
    return data_length;
}

int DummyCongestionController::get_resend_data_length(Context* c, Event* e, WiFuPacket* p) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
    string& send_buffer = e->get_socket()->get_send_buffer();

    // SND.NXT should have previously been set back to SND.UNA
    int num_unsent = (int) send_buffer.size() - (int) ccc->get_num_outstanding();

    // we do not want to make a packet larger than the window size
    int data_length = min(min(num_unsent, (int) p->max_data_length()), min((int) ccc->get_snd_wnd(), MAX_TCP_RECEIVE_WINDOW_SIZE));
    assert(data_length > 0);
    return data_length;
}