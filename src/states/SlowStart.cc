#include "states/SlowStart.h"

SlowStart::SlowStart() {
    cout << "SlowStart Constructor" << endl;
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
    TCPPacket* p = (TCPPacket*)e->get_packet();
    cout << "SlowStart::receive_packet(), packet is ack: " << p->is_naked_ack() << endl;
    cout << "SlowStart::receive_packet(), data: " << (const char*) p->get_data() << endl;

    // Send ACK
    send_data(c, e, p->is_naked_ack());
}

ssize_t SlowStart::send_to(Context* c, SendEvent* e) {
    cout << "SlowStart::send_to()" << endl;
    CongestionControlContext* context = (CongestionControlContext*) c;

    ssize_t data_length = e->data_length();
    unsigned char* buffer = e->get_data();

    IQueue<unsigned char>* queue = &context->get_queue();
    int num_enqueued = 0;

    for (; num_enqueued < data_length; num_enqueued++) {
        if (queue->size() >= context->get_max_buffer_size()) {
            break;
        }
        context->get_queue().enqueue(buffer[num_enqueued]);
    }

    send_data(c, e, false);

    return num_enqueued;
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

    IQueue<unsigned char>* queue = &context->get_queue();

    int max_packet_size = 1400;

    Socket* s = e->get_socket();


    // Build packet and send it.
    TCPPacket* p = new TCPPacket();
    unsigned char data[max_packet_size];

    int num_sent = 0;
    for (; num_sent < max_packet_size; num_sent++) {
        if (queue->isEmpty()) {
            cout << "SlowStart::send_data(), queue is empty" << endl;
            break;
        }
        data[num_sent] = (unsigned char) queue->dequeue();
    }

    AddressPort* source = s->get_local_address_port();
    AddressPort* destination = s->get_remote_address_port();

    p->set_ip_protocol(SIMPLE_TCP);
    p->set_ip_destination_address_s(destination->get_address());
    p->set_ip_source_address_s(source->get_address());

    p->set_destination_port(destination->get_port());
    p->set_source_port(source->get_port());

    p->set_data(data, num_sent);

    cout << "SlowStart::send_data(), num sent: " << num_sent << endl;

    if(received_naked_ack && p->get_data_length_bytes() == 0) {
        cout << "SlowStart::send_data(), returning early" << endl;
        return;
    }

    context->set_can_send_data(false);

    SendPacketEvent* event = new SendPacketEvent(s, p);
    Dispatcher::instance().enqueue(event);
}

