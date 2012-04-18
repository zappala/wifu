#include "protocol/SimpleUDP.h"
#include "Timer.h"

int sent, received;
SimpleUDP::SimpleUDP() : Protocol(UDP) {
    sent = 0;
    received = 0;
}

SimpleUDP& SimpleUDP::instance() {
    static SimpleUDP instance_;
    return instance_;
}

SimpleUDP::~SimpleUDP() {

}


// IContext methods

void SimpleUDP::icontext_socket(QueueProcessor<Event*>* q, SocketEvent* e) {
    map_[e->get_socket()] = new SimpleUDPContainer();
}

void SimpleUDP::icontext_bind(QueueProcessor<Event*>* q, BindEvent* e) {
}

void SimpleUDP::icontext_listen(QueueProcessor<Event*>* q, ListenEvent* e) {

}

void SimpleUDP::icontext_receive_packet(QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
    SimpleUDPContainer* c = map_.find(e->get_socket())->second;
    
    c->get_packet_queue().push(e);
    send_receive_response(c);

    ++received;
}

void SimpleUDP::icontext_send_packet(QueueProcessor<Event*>* q, SendPacketEvent* e) {

}

void SimpleUDP::icontext_connect(QueueProcessor<Event*>* q, ConnectEvent* e) {

}

void SimpleUDP::icontext_accept(QueueProcessor<Event*>* q, AcceptEvent* e) {

}

void SimpleUDP::icontext_new_connection_established(QueueProcessor<Event*>* q, ConnectionEstablishedEvent* e) {

}

void SimpleUDP::icontext_new_connection_initiated(QueueProcessor<Event*>* q, ConnectionInitiatedEvent* e) {

}

void SimpleUDP::icontext_close(QueueProcessor<Event*>* q, CloseEvent* e) {
    ResponseEvent* response_event = ObjectPool<ResponseEvent>::instance().get();
    response_event->set_socket(e->get_socket());
    response_event->set_message_type(e->get_message_type());
    response_event->set_fd(e->get_fd());
    response_event->set_return_value(0);
    response_event->set_errno(0);
    response_event->set_default_length();
    response_event->set_destination(e->get_source());
    dispatch(response_event);

    q->enqueue(new DeleteSocketEvent(e->get_socket()));
}

void SimpleUDP::icontext_timer_fired_event(QueueProcessor<Event*>* q, TimerFiredEvent* e) {

}

void SimpleUDP::icontext_resend_packet(QueueProcessor<Event*>* q, ResendPacketEvent* e) {

}

void SimpleUDP::icontext_send(QueueProcessor<Event*>* q, SendEvent* e) {
    Socket* s = e->get_socket();

    struct SendToMessage* m = (struct SendToMessage*) e->get_buffer();

    AddressPort destination(&(m->addr));
    if (s->get_local_address_port()->get_address() == "0.0.0.0") {
        // bind
        gcstring local = SourceGetter::instance().get_source_address(destination.get_address());
        AddressPort* local_ap = new AddressPort(local, s->get_local_address_port()->get_port());
        s->set_local_address_port(local_ap);
    }

    UDPPacket* p = new UDPPacket();

    p->set_ip_destination_address_s(destination.get_address());
    p->set_destination_port(destination.get_port());

    p->set_ip_source_address_s(s->get_local_address_port()->get_address());
    p->set_source_port(s->get_local_address_port()->get_port());

    p->set_ip_protocol(UDP);

    // do this last as it will compute checksum for us
    p->set_data(e->get_data(), e->get_data_length());

    p->calculate_and_set_udp_checksum();

    dispatch(new NetworkSendPacketEvent(s, p));

    ResponseEvent* response_event = ObjectPool<ResponseEvent>::instance().get();
    response_event->set_socket(s);
    response_event->set_message_type(e->get_message_type());
    response_event->set_fd(e->get_fd());
    response_event->set_return_value(e->get_data_length());
    response_event->set_errno(0);
    response_event->set_default_length();
    response_event->set_destination(e->get_source());
    dispatch(response_event);
    ++sent;
}

bool SimpleUDP::icontext_can_send(Socket* s) {
    return true;
}

bool SimpleUDP::icontext_can_receive(Socket* s) {
    return true;
}

void SimpleUDP::icontext_receive(QueueProcessor<Event*>* q, ReceiveEvent* e) {
    SimpleUDPContainer* c = map_.find(e->get_socket())->second;
    c->set_receive_event(e);
    send_receive_response(c);
}

void SimpleUDP::icontext_receive_buffer_not_empty(QueueProcessor<Event*>* q, ReceiveBufferNotEmptyEvent* e) {

}

void SimpleUDP::icontext_receive_buffer_not_full(QueueProcessor<Event*>* q, ReceiveBufferNotFullEvent* e) {

}

void SimpleUDP::icontext_send_buffer_not_empty(QueueProcessor<Event*>* q, SendBufferNotEmptyEvent* e) {

}

void SimpleUDP::icontext_send_buffer_not_full(QueueProcessor<Event*>* q, SendBufferNotFullEvent* e) {

}

void SimpleUDP::icontext_delete_socket(QueueProcessor<Event*>* q, DeleteSocketEvent* e) {
    map_.erase(e->get_socket());    
}

void SimpleUDP::icontext_set_socket_option(QueueProcessor<Event*>* q, SetSocketOptionEvent* e) {

}

void SimpleUDP::icontext_get_socket_option(QueueProcessor<Event*>* q, GetSocketOptionEvent* e) {

}

void SimpleUDP::send_receive_response(SimpleUDPContainer* c) {
    ReceiveEvent* e = c->get_receive_event();
    
    if (!e) {
        return;
    }

    packet_queue& q = c->get_packet_queue();

    if (q.empty()) {
        return;
    }

    NetworkReceivePacketEvent* nrpe = q.front();
    UDPPacket* p = (UDPPacket*) nrpe->get_packet();
    q.pop();
    Socket* s = e->get_socket();

    RecvFromResponseEvent* response = (RecvFromResponseEvent*) ObjectPool<ResponseEvent>::instance().get();
    response->set_socket(s);
    response->set_message_type(e->get_message_type());
    response->set_fd(e->get_fd());
    // done in set_return_buffer()
    // response->set_return_value(length);
    response->set_errno(0);
    // done in set_return_buffer()
    // response->set_lenth();
    response->set_destination(e->get_source());
    response->set_addr(p->get_source_address_port()->get_network_struct_ptr(), sizeof (struct sockaddr_in));
    response->set_return_buffer(p->get_data(), p->get_data_length_bytes());

    c->set_receive_event(0);
    dispatch(response);

}