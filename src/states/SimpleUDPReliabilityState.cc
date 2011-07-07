/* 
 * File:   SimpleUDPReliabilityState.cc
 * Author: leer
 * 
 * Created on June 21, 2011, 10:18 AM
 */

#include "states/SimpleUDPReliabilityState.h"

SimpleUDPReliabilityState::SimpleUDPReliabilityState() {

}

SimpleUDPReliabilityState::~SimpleUDPReliabilityState() {

}

void SimpleUDPReliabilityState::state_send_packet(Context* c, QueueProcessor<Event*>* q, SendPacketEvent* e) {
//    SimpleUDPReliabilityContext* rc = (SimpleUDPReliabilityContext*) c;
//    //TCPPacket* p = (TCPPacket*) e->get_packet();
//    UDPPacket* p = (UDPPacket*) e->get_packet();
//
//    //TODO: Figure out sending.  Do we need to use a buffer?
//    Socket* s = e->get_socket();
//    p->set_data(s->get_send_buffer());

    //No-op; we already took care of packetizing and queuing up to send, no reliability functionality in UDP
}

void SimpleUDPReliabilityState::state_send_buffer_not_empty(Context* c, QueueProcessor<Event*>* q, SendBufferNotEmptyEvent* e) {
    //cout << "SimpleUDPReliabilityState::state_send_buffer_not_empty(), sending packets" << endl;
    send_packets(c, q, e);
}

void SimpleUDPReliabilityState::send_packets(Context* c, QueueProcessor<Event*>* q, Event* e) {
    //cout << "SimpleUDPReliabilityState::send_packets()" << endl;

    Socket* s = e->get_socket();
    gcstring& send_buffer = s->get_send_buffer();

    while ((int) send_buffer.size() > 0) {

        send_one_packet(c, q, e);
    }
}

void SimpleUDPReliabilityState::send_one_packet(Context* c, QueueProcessor<Event*>* q, Event* e) {

    //cout << "SimpleUDPReliabilityState::send_one_packet()" << endl;
    //SimpleUDPReliabilityState* ccc = (SimpleUDPReliabilityState*) c;
    Socket* s = e->get_socket();

    gcstring& send_buffer = s->get_send_buffer();

    UDPPacket* p = new UDPPacket();
    //p->insert_tcp_header_option(new TCPTimestampOption());

    int data_length = min(p->max_data_length(), (int)send_buffer.size());

    assert(send_buffer.size() > 0);

    //We want the beginning of the buffer, no matter what.
    const char* data = send_buffer.data();

    //ccc->set_snd_nxt(ccc->get_snd_nxt() + data_length);

    //cout << "SimpleUDPReliabilityState::send_one_packet(): remote address = " << s->get_remote_address_port()->get_address() << endl;
    //cout << "SimpleUDPReliabilityState::send_one_packet(): remote port = " << s->get_remote_address_port()->get_port() << endl;

    AddressPort* destination = s->get_remote_address_port();
    AddressPort* source = s->get_local_address_port();

    p->set_ip_destination_address_s(destination->get_address());
    p->set_destination_port(destination->get_port());

    p->set_ip_source_address_s(source->get_address());
    p->set_source_port(source->get_port());

    p->set_data((unsigned char*) data, data_length);

    assert(p->get_data_length_bytes() > 0);

    q->enqueue(new SendPacketEvent(s, p));
    
    //delete the data from the buffer, we don't resend
    s->get_send_buffer().erase(0, data_length);

    q->enqueue(new SendBufferNotFullEvent(s));
}

void SimpleUDPReliabilityState::state_timer_fired(Context* c, QueueProcessor<Event*>* q, TimerFiredEvent* e) {
    //noop
}

void SimpleUDPReliabilityState::state_receive_packet(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    //cout << "SimpleUDPReliabilityState::state_receive_packet(): received packet.\n";
    UDPPacket* p = (UDPPacket*) e->get_packet();

    //TODO: How do we return data?

    //ResponseEvent* re = new ResponseEvent();
    //re->

    handle_data(c, q, e);
}

void SimpleUDPReliabilityState::state_receive_buffer_not_empty(Context* c, QueueProcessor<Event*>* q, ReceiveBufferNotEmptyEvent* e) {
    SimpleUDPReliabilityContext* rc = (SimpleUDPReliabilityContext*) c;
    Socket* s = e->get_socket();

    if (rc->get_receive_event() && !s->get_receive_buffer().empty()) {
        //cout << "SimpleUDPReliabilityState::state_receive_buffer_not_empty(): dispatching data to saved receive event marker.\n";
        create_and_dispatch_received_data(c, q, rc->get_receive_event());
        rc->set_receive_event(0);
    }
}

void SimpleUDPReliabilityState::state_receive(Context* c, QueueProcessor<Event*>* q, ReceiveEvent* e) {
    SimpleUDPReliabilityContext* rc = (SimpleUDPReliabilityContext*) c;
    Socket* s = e->get_socket();

    if (!s->get_receive_buffer().empty()) {
        //cout << "SimpleUDPReliabilityState::state_receive(): buffer not empty.\n";
        create_and_dispatch_received_data(c, q, e);
    } else {
        //cout << "SimpleUDPReliabilityState::state_receive(): buffer empty, setting a receive event marker.\n";
        assert(!rc->get_receive_event());
        rc->set_receive_event(e);
    }
}

void SimpleUDPReliabilityState::create_and_dispatch_received_data(Context* c, QueueProcessor<Event*>* q, ReceiveEvent* e) {
    //SimpleUDPReliabilityContext* rc = (SimpleUDPReliabilityContext*) c;
    Socket* s = e->get_socket();
    int buffer_size = e->get_receive_buffer_size();

    // TODO: change this to use gcstring::data() so we avoid the copy in substr
    gcstring data = s->get_receive_buffer().substr(0, buffer_size);
    int length = data.size();
    s->get_receive_buffer().erase(0, length);

    //rc->set_rcv_wnd(rc->get_rcv_wnd() + length);

    ResponseEvent* response = new ResponseEvent(s, e->get_name(), e->get_map()[FILE_STRING]);
    response->put(BUFFER_STRING, data);
    response->put(ADDRESS_STRING, s->get_remote_address_port()->get_address());
    response->put(PORT_STRING, Utils::itoa(s->get_remote_address_port()->get_port()));
    response->put(RETURN_VALUE_STRING, Utils::itoa(data.size()));
    response->put(ERRNO, Utils::itoa(0));

    //cout << "SimpleUDPReliabilityState::create_and_dispatch_received_data(): dispatching ResponseEvent.\n";
    Dispatcher::instance().enqueue(response);
    q->enqueue(new ReceiveBufferNotFullEvent(s));
}

void SimpleUDPReliabilityState::handle_data(Context* c, QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    //SimpleUDPReliabilityContext* rc = (SimpleUDPReliabilityContext*) c;
    UDPPacket* p = (UDPPacket*) e->get_packet();
    Socket* s = e->get_socket();

    //cout << "SimpleUDPReliabilityState::handle_data(): putting packet data in receive buffer.\n";

    gcstring& receive_buffer = s->get_receive_buffer();
    u_int32_t before_rcv_buffer_size = receive_buffer.size();
    receive_buffer.append((const char*)p->get_data());
    u_int32_t after_receive_buffer_size = receive_buffer.size();
    u_int32_t amount_put_in_receive_buffer = after_receive_buffer_size - before_rcv_buffer_size;
    assert(amount_put_in_receive_buffer >= 0);

    //cout << "SimpleUDPReliabilityState::handle_data(): Data size = " << amount_put_in_receive_buffer << endl;
    if (amount_put_in_receive_buffer > 0) {
        //cout << "SimpleUDPReliabilityState::handle_data(): enquing ReceiveBufferNotEmptyEvent.\n";
        q->enqueue(new ReceiveBufferNotEmptyEvent(s));
    }
}
