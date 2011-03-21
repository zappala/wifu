#include "states/SlowStart.h"

SlowStart::SlowStart() {
    //    cout << "SlowStart Constructor" << endl;
}

SlowStart::~SlowStart() {

}

void SlowStart::send_packet(Context* c, SendPacketEvent* e) {
//    cout << "SlowStart::send_packet(), socket: " << e->get_socket() << endl;

}

void SlowStart::receive_packet(Context* c, NetworkReceivePacketEvent* e) {
    cout << "SlowStart::receive_packet(), socket: " << e->get_socket() << endl;
    CongestionControlContext* context = (CongestionControlContext*) c;
    context->set_can_send_data(true);
    TCPPacket* p = (TCPPacket*) e->get_packet();
//    cout << "SlowStart::receive_packet(), packet is naked ack: " << p->is_naked_ack() << endl;
//    cout << "SlowStart::receive_packet(), packet is ack: " << p->is_tcp_ack() << endl;
//    cout << "SlowStart::receive_packet(), data: " << (const char*) p->get_data() << endl;

    // Send ACK
    send_data(c, e, p->is_naked_ack());
}

void SlowStart::send_to(Context* c, SendEvent* e) {
//    cout << "SlowStart::send_to()" << endl;
    CongestionControlContext* context = (CongestionControlContext*) c;
    Socket* s = e->get_socket();

    if(s->get_send_buffer().size() + e->data_length() > MAX_BUFFER_SIZE) {
        // No room in the inn
        assert(context->get_buffered_send_event() == NULL);
        context->set_buffered_send_event(e);
        return;
    }
//    cout << "SlowStart::send_to(), putting data immediately in send buffer" << endl;
    append_data_to_send_buffer(c, e);
    send_data(c, e, false);
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
//        cout << "SlowStart::send_data(), cannot send data" << endl;
        return;
    }

    Socket* s = e->get_socket();

    // Build packet and send it.
    TCPPacket* p = new TCPPacket();

    string data = s->get_send_buffer().substr(0, p->max_data_length());
    s->get_send_buffer().erase(0, p->max_data_length());

    // TODO: append any blocking send_to call
    SendEvent* send_event = context->get_buffered_send_event();
    if(send_event != NULL && s->get_send_buffer().size() + send_event->data_length() <= MAX_BUFFER_SIZE) {
//        cout << "SlowStart::send_data(), putting saved send data in send buffer" << endl;
        append_data_to_send_buffer(c, send_event);
        context->set_buffered_send_event(NULL);
    }

    AddressPort* source = s->get_local_address_port();
    AddressPort* destination = s->get_remote_address_port();

    p->set_ip_protocol(SIMPLE_TCP);
    p->set_ip_destination_address_s(destination->get_address());
    p->set_ip_source_address_s(source->get_address());

    p->set_destination_port(destination->get_port());
    p->set_source_port(source->get_port());

    p->set_data((unsigned char*) data.c_str(), data.length());

//    cout << "SlowStart::send_data(), num sent: " << data.length() << endl;
//    cout << "SlowStart::send_data(), data: " << data << endl;

    if (received_naked_ack && p->get_data_length_bytes() == 0) {
        cout << "SlowStart::send_data(), returning early" << endl;
        return;
    }

    context->set_can_send_data(false);

    cout << "SlowStart::send_data(), sending a packet:" << endl;
    cout << p->to_s() << endl;
    SendPacketEvent* event = new SendPacketEvent(s, p);
    Dispatcher::instance().enqueue(event);
}

void SlowStart::append_data_to_send_buffer(Context* c, SendEvent* e) {
    Socket* s = e->get_socket();
    int size = e->data_length();
    s->get_send_buffer().append((const char*) e->get_data(), size);

    // TODO: figure out other errors
    ResponseEvent* response = new ResponseEvent(s, e->get_name(), e->get_map()[FILE_STRING]);
    response->put(RETURN_VALUE_STRING, Utils::itoa(size));
    response->put(ERRNO, Utils::itoa(0));
    Dispatcher::instance().enqueue(response);
}

