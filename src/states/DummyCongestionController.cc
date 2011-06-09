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

        send_probe_packet(c, q, e);
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
    TCPPacket* p = (TCPPacket*) e->get_packet();

    u_int32_t length = (p->is_tcp_syn() || p->is_tcp_fin()) ? 1 : p->get_data_length_bytes();

    ccc->set_snd_nxt(ccc->get_snd_una() + length);

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

    // Represents the first byte in the send buffer which has "never" been sent before.
    // We actually may have sent data already but if SND.UNA changes due to a drop we will treat it as if we never sent it.
    int index = ccc->get_num_outstanding();
    cout << "DummyCongestionController::send_packets(), num outstanding:  " << ccc->get_num_outstanding() << endl;
    cout << "DummyCongestionController::send_packets(), send window size: " << ccc->get_snd_wnd() << endl;
    assert(ccc->get_num_outstanding() <= ccc->get_snd_wnd());


    int num_unsent = send_buffer.size() - index;

    while (num_unsent > 0 && ccc->get_num_outstanding() < ccc->get_snd_wnd()) {

        TCPPacket* p = new TCPPacket();
        p->insert_tcp_header_option(new TCPTimestampOption());

        int available_window_space = (int) ccc->get_snd_wnd() - (int) ccc->get_num_outstanding();

        u_int32_t data_length = min(min(available_window_space, num_unsent), (int) p->max_data_length());
        cout << "Data length: " << data_length << endl;
        assert(data_length > 0);
        assert(send_buffer.size() > 0);


        const char* data = (send_buffer.data() + index);

        num_unsent -= data_length;
        index += data_length;
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
}

void DummyCongestionController::send_probe_packet(Context* c, QueueProcessor<Event*>* q, Event* e) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
    Socket* s = e->get_socket();

    string& send_buffer = s->get_send_buffer();

    // Represents the first byte in the send buffer which has "never" been sent before.
    // We actually may have sent data already but if SND.UNA changes due to a drop we will treat it as if we never sent it.
    int index = ccc->get_num_outstanding();
    int num_unsent = send_buffer.size() - index;

    assert(num_unsent > 0);

    TCPPacket* p = new TCPPacket();
    p->insert_tcp_header_option(new TCPTimestampOption());

    const char* data = (send_buffer.data() + index);

    u_int32_t data_length = 1;
    index += data_length;
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

