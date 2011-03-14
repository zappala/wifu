#include "states/SlowStart.h"

SlowStart::SlowStart() {
    //    cout << "SlowStart Constructor" << endl;
}

SlowStart::~SlowStart() {

}

void SlowStart::send_packet(Context* c, SendPacketEvent* e) {
    cout << "SlowStart::send_packet(), socket: " << e->get_socket() << endl;

}

void SlowStart::receive_packet(Context* c, NetworkReceivePacketEvent* e) {
    cout << "SlowStart::receive_packet(), socket: " << e->get_socket() << endl;
    CongestionControlContext* context = (CongestionControlContext*) c;
    context->set_can_send_data(true);
    TCPPacket* p = (TCPPacket*) e->get_packet();
    cout << "SlowStart::receive_packet(), packet is naked ack: " << p->is_naked_ack() << endl;
    cout << "SlowStart::receive_packet(), packet is ack: " << p->is_tcp_ack() << endl;
    cout << "SlowStart::receive_packet(), data: " << (const char*) p->get_data() << endl;

    // Send ACK
    send_data(c, e, p->is_naked_ack());
}

ssize_t SlowStart::send_to(Context* c, SendEvent* e) {
    cout << "SlowStart::send_to()" << endl;
    CongestionControlContext* context = (CongestionControlContext*) c;
    Socket* s = e->get_socket();

    // Currently we are enqueuing everything
    // Enforce max send size length here
    //        if (send_buffer->size() > 10000) {
    //            cout << "SlowStart::send_to(), breaking early" << endl;
    //            return;
    //        }

    string data((const char*) e->get_data(), e->data_length());
    s->get_send_buffer().append(data);

    cout << "SlowStart::send_to(): enqueing: " << data << endl;

    send_data(c, e, false);

    return data.length();
}

void SlowStart::enter(Context* c) {
    enter_state("SlowStart");
}

void SlowStart::exit(Context* c) {
    leave_state("SlowStart");
}

void SlowStart::send_data(Context* c, Event* e, bool received_naked_ack) {
    cout << "SlowStart::send_data()" << endl;
    CongestionControlContext* context = (CongestionControlContext*) c;

    if (!context->can_send_data()) {
        // wait for an ack
        cout << "SlowStart::send_data(), cannot send data" << endl;
        return;
    }

    Socket* s = e->get_socket();

    // Build packet and send it.
    TCPPacket* p = new TCPPacket();

    string data = s->get_send_buffer().substr(0, p->max_data_length());
    s->get_send_buffer().erase(0, p->max_data_length());

    AddressPort* source = s->get_local_address_port();
    AddressPort* destination = s->get_remote_address_port();

    p->set_ip_protocol(SIMPLE_TCP);
    p->set_ip_destination_address_s(destination->get_address());
    p->set_ip_source_address_s(source->get_address());

    p->set_destination_port(destination->get_port());
    p->set_source_port(source->get_port());

    p->set_data((unsigned char*) data.c_str(), data.length());

    cout << "SlowStart::send_data(), num sent: " << data.length() << endl;

    if (received_naked_ack && p->get_data_length_bytes() == 0) {
        cout << "SlowStart::send_data(), returning early" << endl;
        return;
    }

    context->set_can_send_data(false);

    SendPacketEvent* event = new SendPacketEvent(s, p);
    Dispatcher::instance().enqueue(event);
}

