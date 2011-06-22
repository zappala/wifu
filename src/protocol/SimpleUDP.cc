/* 
 * File:   SimpleUDP.cc
 * Author: leer
 * 
 * Created on June 20, 2011, 4:51 PM
 */

#include "protocol/SimpleUDP.h"
#include "MockNetworkInterface.h"

SimpleUDP::SimpleUDP(int protocol) : Protocol(protocol) {
    /*states_we_can_send_ack_.insert(type_name(Established));
    states_we_can_send_ack_.insert(type_name(FinWait1));
    states_we_can_send_ack_.insert(type_name(FinWait2));
    states_we_can_send_ack_.insert(type_name(CloseWait));
    states_we_can_send_ack_.insert(type_name(Closing));
    states_we_can_send_ack_.insert(type_name(LastAck));
    states_we_can_send_ack_.insert(type_name(TimeWait));*/
}

SimpleUDP::~SimpleUDP() {

}

SimpleUDP& SimpleUDP::instance() {
    static SimpleUDP instance_;
    return instance_;
}

void SimpleUDP::icontext_socket(QueueProcessor<Event*>* q, SocketEvent* e) {
    //    cout << "SimpleUDP::icontext_socket()" << endl;
    Socket* s = e->get_socket();
    map_[s] = new SimpleUDPIContextContainer();

    SimpleUDPIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_socket(q, e);
    //c->get_connection_manager()->icontext_socket(q, e);
    //c->get_congestion_control()->icontext_socket(q, e);
}

void SimpleUDP::icontext_bind(QueueProcessor<Event*>* q, BindEvent* e) {
    Socket* s = e->get_socket();
    SimpleUDPIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_bind(q, e);
    //c->get_connection_manager()->icontext_bind(q, e);
    //c->get_congestion_control()->icontext_bind(q, e);
}

void SimpleUDP::icontext_listen(QueueProcessor<Event*>* q, ListenEvent* e) {
    Socket* s = e->get_socket();
    SimpleUDPIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_listen(q, e);
    //c->get_connection_manager()->icontext_listen(q, e);
    //c->get_congestion_control()->icontext_listen(q, e);
}

void SimpleUDP::icontext_receive_packet(QueueProcessor<Event*>* q, NetworkReceivePacketEvent* e) {
//    cout << "SimpleUDP::icontext_receive_packet(): " << endl;

    Socket* s = e->get_socket();
    SimpleUDPIContextContainer* c = map_.find(s)->second;
    TCPPacket* p = (TCPPacket*) e->get_packet();
    SimpleUDPReliabilityContext* rc = (SimpleUDPReliabilityContext*) c->get_reliability();
    //ConnectionManagerContext* cmc = (ConnectionManagerContext*) c->get_connection_manager();
    //    cout << p->to_s() << endl;

    rc->icontext_receive_packet(q, e);

}

void SimpleUDP::icontext_send_packet(QueueProcessor<Event*>* q, SendPacketEvent* e) {
    Socket* s = e->get_socket();
    SimpleUDPIContextContainer* c = map_.find(s)->second;
    TCPPacket* p = (TCPPacket*) e->get_packet();

    c->get_reliability()->icontext_send_packet(q, e);
    //c->get_connection_manager()->icontext_send_packet(q, e);
    //c->get_congestion_control()->icontext_send_packet(q, e);

//        cout << "SimpleUDP::icontext_send_packet(): " << endl;
    //    cout << p->to_s() << endl;

    //TODO: Support rate limiters!
    send_network_packet(e->get_socket(), p);
}

void SimpleUDP::icontext_connect(QueueProcessor<Event*>* q, ConnectEvent* e) {
    Socket* s = e->get_socket();
    SimpleUDPIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_connect(q, e);
    //c->get_connection_manager()->icontext_connect(q, e);
    //c->get_congestion_control()->icontext_connect(q, e);
}

void SimpleUDP::icontext_accept(QueueProcessor<Event*>* q, AcceptEvent* e) {
    Socket* s = e->get_socket();
    SimpleUDPIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_accept(q, e);
    //c->get_connection_manager()->icontext_accept(q, e);
    //c->get_congestion_control()->icontext_accept(q, e);
}

void SimpleUDP::icontext_new_connection_established(QueueProcessor<Event*>* q, ConnectionEstablishedEvent* e) {
    Socket* s = e->get_socket();
    SimpleUDPIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_new_connection_established(q, e);
    //c->get_connection_manager()->icontext_new_connection_established(q, e);
    //c->get_congestion_control()->icontext_new_connection_established(q, e);
}

void SimpleUDP::icontext_new_connection_initiated(QueueProcessor<Event*>* q, ConnectionInitiatedEvent* e) {
//    Socket* listening_socket = e->get_socket();
//    Socket* new_socket = e->get_new_socket();
//
//    SimpleUDPIContextContainer* listening_cc = map_.find(listening_socket)->second;
//    map_[new_socket] = listening_cc;
//
//    SimpleUDPIContextContainer* new_cc = new SimpleUDPIContextContainer();
//    map_[listening_socket] = new_cc;
//
//    new_cc->get_reliability()->icontext_new_connection_initiated(q, e);
//    new_cc->get_connection_manager()->icontext_new_connection_initiated(q, e);
//    new_cc->get_congestion_control()->icontext_new_connection_initiated(q, e);
}

void SimpleUDP::icontext_close(QueueProcessor<Event*>* q, CloseEvent* e) {
//    Socket* s = e->get_socket();
//    SimpleUDPIContextContainer* c = map_.find(s)->second;
//
//
//    if (s->get_send_buffer().empty()) {
//        c->get_connection_manager()->icontext_close(q, e);
//    } else {
//        c->set_saved_close_event(e);
//    }
//
//    ResponseEvent* response = new ResponseEvent(s, e->get_name(), e->get_map()[FILE_STRING]);
//    response->put(RETURN_VALUE_STRING, Utils::itoa(0));
//    response->put(ERRNO, Utils::itoa(0));
//    dispatch(response);
}

void SimpleUDP::icontext_timer_fired_event(QueueProcessor<Event*>* q, TimerFiredEvent* e) {
    Socket* s = e->get_socket();
    SimpleUDPIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_timer_fired_event(q, e);
    //c->get_connection_manager()->icontext_timer_fired_event(q, e);
    //c->get_congestion_control()->icontext_timer_fired_event(q, e);
}

void SimpleUDP::icontext_resend_packet(QueueProcessor<Event*>* q, ResendPacketEvent* e) {
//    cout << "SimpleUDP::icontext_resend_packet()" << endl;

//    Socket* s = e->get_socket();
//    SimpleUDPIContextContainer* c = map_.find(s)->second;
//
//    c->get_congestion_control()->icontext_resend_packet(q, e);
//    c->get_reliability()->icontext_resend_packet(q, e);
//    c->get_connection_manager()->icontext_resend_packet(q, e);
}

void SimpleUDP::icontext_send(QueueProcessor<Event*>* q, SendEvent* e) {
    Socket* s = e->get_socket();
    SimpleUDPIContextContainer* c = map_.find(s)->second;

    if (is_room_in_send_buffer(e)) {
        save_in_buffer_and_send_events(q, e);
    } else {
        //Just drop the packet
        return;
    }

    c->get_reliability()->icontext_send(q, e);
    //c->get_connection_manager()->icontext_send(q, e);
    //c->get_congestion_control()->icontext_send(q, e);
}

void SimpleUDP::icontext_receive(QueueProcessor<Event*>* q, ReceiveEvent* e) {
    Socket* s = e->get_socket();
    SimpleUDPIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_receive(q, e);
    //c->get_connection_manager()->icontext_receive(q, e);
    //c->get_congestion_control()->icontext_receive(q, e);
}

void SimpleUDP::icontext_receive_buffer_not_empty(QueueProcessor<Event*>* q, ReceiveBufferNotEmptyEvent* e) {
    Socket* s = e->get_socket();
    SimpleUDPIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_receive_buffer_not_empty(q, e);
    //c->get_connection_manager()->icontext_receive_buffer_not_empty(q, e);
    //c->get_congestion_control()->icontext_receive_buffer_not_empty(q, e);
}

void SimpleUDP::icontext_receive_buffer_not_full(QueueProcessor<Event*>* q, ReceiveBufferNotFullEvent* e) {
//    cout << "SimpleUDP::icontext_receive_buffer_not_full()" << endl;
    Socket* s = e->get_socket();
    SimpleUDPIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_receive_buffer_not_full(q, e);
    //c->get_connection_manager()->icontext_receive_buffer_not_full(q, e);
    //c->get_congestion_control()->icontext_receive_buffer_not_full(q, e);
}

void SimpleUDP::icontext_send_buffer_not_empty(QueueProcessor<Event*>* q, SendBufferNotEmptyEvent* e) {
    Socket* s = e->get_socket();
    SimpleUDPIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_send_buffer_not_empty(q, e);
    //c->get_connection_manager()->icontext_send_buffer_not_empty(q, e);
    //c->get_congestion_control()->icontext_send_buffer_not_empty(q, e);
}

void SimpleUDP::icontext_send_buffer_not_full(QueueProcessor<Event*>* q, SendBufferNotFullEvent* e) {
    Socket* s = e->get_socket();
    SimpleUDPIContextContainer* c = map_.find(s)->second;

    SendEvent* saved_send_event = c->get_saved_send_event();

    if (saved_send_event && is_room_in_send_buffer(saved_send_event)) {
        save_in_buffer_and_send_events(q, saved_send_event);
    }

    c->get_reliability()->icontext_send_buffer_not_full(q, e);
    //c->get_connection_manager()->icontext_send_buffer_not_full(q, e);
    //c->get_congestion_control()->icontext_send_buffer_not_full(q, e);
}

bool SimpleUDP::icontext_can_send(Socket* s) {
    //SimpleUDPIContextContainer* c = map_.find(s)->second;
    //return c->get_connection_manager()->icontext_can_send(s);
    return true;
}

bool SimpleUDP::icontext_can_receive(Socket* s) {
    //SimpleUDPIContextContainer* c = map_.find(s)->second;
    //return c->get_connection_manager()->icontext_can_receive(s);
    return true;
}

void SimpleUDP::icontext_delete_socket(QueueProcessor<Event*>* q, DeleteSocketEvent* e) {
    Socket* s = e->get_socket();
    SimpleUDPIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_delete_socket(q, e);
    //c->get_connection_manager()->icontext_delete_socket(q, e);
    //c->get_congestion_control()->icontext_delete_socket(q, e);

    map_.erase(s);
    assert(map_.find(s) == map_.end());
}

void SimpleUDP::icontext_set_socket_option(QueueProcessor<Event*>* q, SetSocketOptionEvent* e) {
    Socket* s = e->get_socket();
    SimpleUDPIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_set_socket_option(q, e);
    //c->get_connection_manager()->icontext_set_socket_option(q, e);
    //c->get_congestion_control()->icontext_set_socket_option(q, e);
}

void SimpleUDP::icontext_get_socket_option(QueueProcessor<Event*>* q, GetSocketOptionEvent* e) {
    Socket* s = e->get_socket();
    SimpleUDPIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_get_socket_option(q, e);
    //c->get_connection_manager()->icontext_get_socket_option(q, e);
    //c->get_congestion_control()->icontext_get_socket_option(q, e);
}

bool SimpleUDP::is_room_in_send_buffer(SendEvent* e) {
    Socket* s = e->get_socket();
    int num_bytes_to_send = e->data_length();
    return s->get_send_buffer().size() + num_bytes_to_send <= MAX_BUFFER_SIZE;
}

void SimpleUDP::save_in_buffer_and_send_events(QueueProcessor<Event*>* q, SendEvent* e) {
    int num_bytes_to_send = e->data_length();
    const char* data = (const char*) e->get_data();

    Socket* s = e->get_socket();
    s->get_send_buffer().append(data, num_bytes_to_send);

    ResponseEvent* response = new ResponseEvent(s, e->get_name(), e->get_map()[FILE_STRING]);
    response->put(RETURN_VALUE_STRING, Utils::itoa(num_bytes_to_send));
    response->put(ERRNO, Utils::itoa(0));

    dispatch(response);
    q->enqueue(new SendBufferNotEmptyEvent(s));
}

void SimpleUDP::create_and_dispatch_received_data(QueueProcessor<Event*>* q, ReceiveEvent* e) {
    Socket* s = e->get_socket();
    int buffer_size = e->get_receive_buffer_size();

    string data = s->get_receive_buffer().substr(0, buffer_size);
    s->get_receive_buffer().erase(0, data.size());

    ResponseEvent* response = new ResponseEvent(s, e->get_name(), e->get_map()[FILE_STRING]);
    response->put(BUFFER_STRING, data);
    response->put(ADDRESS_STRING, s->get_remote_address_port()->get_address());
    response->put(PORT_STRING, Utils::itoa(s->get_remote_address_port()->get_port()));
    response->put(RETURN_VALUE_STRING, Utils::itoa(data.size()));
    response->put(ERRNO, Utils::itoa(0));

    dispatch(response);
    q->enqueue(new ReceiveBufferNotFullEvent(s));
}
