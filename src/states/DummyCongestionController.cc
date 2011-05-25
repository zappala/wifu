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
        ccc->set_snd_nxt(ccc->get_iss() + 1);
    } else if (p->is_tcp_fin()) {
        ccc->set_snd_nxt(ccc->get_snd_nxt() + 1);
    }
    // we will set snd.nxt for data when we originally send data

//    cout << "DummyCongestionController::state_send_packet()" << endl;
//    cout << "SND.NXT: " << ccc->get_snd_nxt() << endl;
//    cout << "SND.UNA: " << ccc->get_snd_una() << endl;

}

void DummyCongestionController::state_resend_packet(Context* c, ResendPacketEvent* e) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
    TCPPacket* p = (TCPPacket*) e->get_packet();

    u_int32_t length = (p->is_tcp_syn() || p->is_tcp_fin()) ? 1 : p->get_data_length_bytes();

    ccc->set_snd_nxt(ccc->get_snd_una() + length);

//    cout << "DummyCongestionController::state_resend_packet()" << endl;
//    cout << "SND.NXT: " << ccc->get_snd_nxt() << endl;
//    cout << "SND.UNA: " << ccc->get_snd_una() << endl;

    // TODO: resize the window?
}

void DummyCongestionController::state_receive_packet(Context* c, NetworkReceivePacketEvent* e) {
    cout << "DummyCongestionContrller::state_receive_packet()" << endl;
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
            //            cout << "Send window updated on socket: " << e->get_socket() << endl;
        }
    }

//    cout << "DummyCongestionController::state_recieve_packet()" << endl;
//    cout << "SND.NXT: " << ccc->get_snd_nxt() << endl;
//    cout << "SND.UNA: " << ccc->get_snd_una() << endl;



    send_packets(c, e);
}

void DummyCongestionController::state_send_buffer_not_empty(Context* c, SendBufferNotEmptyEvent* e) {
    send_packets(c, e);
}

void DummyCongestionController::send_packets(Context* c, Event* e) {
    TCPTahoeCongestionControlContext* ccc = (TCPTahoeCongestionControlContext*) c;
    Socket* s = e->get_socket();
    string& send_buffer = s->get_send_buffer();

    // Represents the first byte in the send buffer which has "never" been sent before.
    // We actually may have sent data already but if SND.UNA changes due to a drop we will treat it as if we never sent it.
    int index = ccc->get_num_outstanding();
    // number of bytes never sent
    int size = send_buffer.size() - index;

    while (size > 0 && ccc->get_num_outstanding() < ccc->get_snd_wnd()) {

        TCPPacket* p = new TCPPacket();
        p->insert_tcp_header_option(new TCPTimestampOption());

        u_int32_t data_length = min(min((int) ccc->get_snd_wnd(), size), (int) p->max_data_length());

        const char* data = (send_buffer.data() + index);

        size -= data_length;
        index += data_length;
        ccc->set_snd_nxt(ccc->get_snd_nxt() + data_length);

        AddressPort* destination = s->get_remote_address_port();
        AddressPort* source = s->get_local_address_port();

        p->set_ip_destination_address_s(destination->get_address());
        p->set_destination_port(destination->get_port());

        p->set_ip_source_address_s(source->get_address());
        p->set_source_port(source->get_port());

        p->set_data((unsigned char*) data, data_length);

        Dispatcher::instance().enqueue(new SendPacketEvent(s, p));
        Dispatcher::instance().enqueue(new SendBufferNotFullEvent(s));
    }
}

