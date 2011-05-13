#include "protocol/TCPTahoe.h"

TCPTahoe::TCPTahoe(int protocol) : Protocol(protocol) {

}

TCPTahoe::~TCPTahoe() {

}

TCPTahoe& TCPTahoe::instance() {
    static TCPTahoe instance_;
    return instance_;
}

void TCPTahoe::icontext_socket(SocketEvent* e) {
    cout << "TCPTahoe::icontext_socket()" << endl;
    Socket* s = e->get_socket();
    map_[s] = new TCPTahoeIContextContainer();

    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_socket(e);
    c->get_connection_manager()->icontext_socket(e);
    c->get_congestion_control()->icontext_socket(e);
}

void TCPTahoe::icontext_bind(BindEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_bind(e);
    c->get_connection_manager()->icontext_bind(e);
    c->get_congestion_control()->icontext_bind(e);
}

void TCPTahoe::icontext_listen(ListenEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_listen(e);
    c->get_connection_manager()->icontext_listen(e);
    c->get_congestion_control()->icontext_listen(e);
}

void TCPTahoe::icontext_receive_packet(NetworkReceivePacketEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_receive_packet(e);
    c->get_connection_manager()->icontext_receive_packet(e);
    c->get_congestion_control()->icontext_receive_packet(e);
}

void TCPTahoe::icontext_send_packet(SendPacketEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_send_packet(e);
    c->get_connection_manager()->icontext_send_packet(e);
    c->get_congestion_control()->icontext_send_packet(e);

    send_network_packet(e->get_socket(), e->get_packet());
}

void TCPTahoe::icontext_connect(ConnectEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_connect(e);
    c->get_connection_manager()->icontext_connect(e);
    c->get_congestion_control()->icontext_connect(e);
}

void TCPTahoe::icontext_accept(AcceptEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_accept(e);
    c->get_connection_manager()->icontext_accept(e);
    c->get_congestion_control()->icontext_accept(e);
}

void TCPTahoe::icontext_new_connection_established(ConnectionEstablishedEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_new_connection_established(e);
    c->get_connection_manager()->icontext_new_connection_established(e);
    c->get_congestion_control()->icontext_new_connection_established(e);
}

void TCPTahoe::icontext_new_connection_initiated(ConnectionInitiatedEvent* e) {
    Socket* listening_socket = e->get_socket();
    Socket* new_socket = e->get_new_socket();

    TCPTahoeIContextContainer* listening_cc = map_.find(listening_socket)->second;
    map_[new_socket] = listening_cc;

    TCPTahoeIContextContainer* new_cc = new TCPTahoeIContextContainer();
    map_[listening_socket] = new_cc;

    // TODO: Move cache over?

    new_cc->get_reliability()->icontext_new_connection_initiated(e);
    new_cc->get_connection_manager()->icontext_new_connection_initiated(e);
    new_cc->get_congestion_control()->icontext_new_connection_initiated(e);
}

void TCPTahoe::icontext_close(CloseEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    if(s->get_send_buffer().empty()) {
        c->get_connection_manager()->icontext_close(e);
    }
    else {
        c->set_saved_close_event(e);
    }

    ResponseEvent* response = new ResponseEvent(s, e->get_name(), e->get_map()[FILE_STRING]);
    response->put(RETURN_VALUE_STRING, Utils::itoa(0));
    response->put(ERRNO, Utils::itoa(0));
    dispatch(response);
}

void TCPTahoe::icontext_timer_fired_event(TimerFiredEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_timer_fired_event(e);
    c->get_connection_manager()->icontext_timer_fired_event(e);
    c->get_congestion_control()->icontext_timer_fired_event(e);
}

void TCPTahoe::icontext_resend_packet(ResendPacketEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_resend_packet(e);
    c->get_connection_manager()->icontext_resend_packet(e);
    c->get_congestion_control()->icontext_resend_packet(e);

    send_network_packet(e->get_socket(), e->get_packet());
}

void TCPTahoe::icontext_send(SendEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    if (is_room_in_send_buffer(e)) {
        save_in_buffer_and_send_events(e);
    } else {
        c->set_saved_send_event(e);
    }

    //    c->get_reliability()->icontext_send(e);
    //    c->get_connection_manager()->icontext_send(e);
    //    c->get_congestion_control()->icontext_send(e);
}

void TCPTahoe::icontext_receive(ReceiveEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

//    c->get_reliability()->icontext_receive(e);
//    c->get_connection_manager()->icontext_receive(e);
//    c->get_congestion_control()->icontext_receive(e);
}

void TCPTahoe::icontext_receive_buffer_not_empty(ReceiveBufferNotEmptyEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_receive_buffer_not_empty(e);
    c->get_connection_manager()->icontext_receive_buffer_not_empty(e);
    c->get_congestion_control()->icontext_receive_buffer_not_empty(e);
}

void TCPTahoe::icontext_receive_buffer_not_full(ReceiveBufferNotFullEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_receive_buffer_not_full(e);
    c->get_connection_manager()->icontext_receive_buffer_not_full(e);
    c->get_congestion_control()->icontext_receive_buffer_not_full(e);
}

void TCPTahoe::icontext_send_buffer_not_empty(SendBufferNotEmptyEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_send_buffer_not_empty(e);
    c->get_connection_manager()->icontext_send_buffer_not_empty(e);
    c->get_congestion_control()->icontext_send_buffer_not_empty(e);
}

void TCPTahoe::icontext_send_buffer_not_full(SendBufferNotFullEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    SendEvent* saved_send_event = c->get_saved_send_event();

    if (saved_send_event && is_room_in_send_buffer(saved_send_event)) {
        save_in_buffer_and_send_events(saved_send_event);
    }

    //    c->get_reliability()->icontext_send_buffer_not_full(e);
    //    c->get_connection_manager()->icontext_send_buffer_not_full(e);
    //    c->get_congestion_control()->icontext_send_buffer_not_full(e);
}

bool TCPTahoe::icontext_can_send(Socket* s) {
    TCPTahoeIContextContainer* c = map_.find(s)->second;
    return c->get_connection_manager()->icontext_can_send(s);
}

bool TCPTahoe::icontext_can_receive(Socket* s) {
    TCPTahoeIContextContainer* c = map_.find(s)->second;
    return c->get_connection_manager()->icontext_can_receive(s);
}

void TCPTahoe::icontext_delete_socket(DeleteSocketEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_delete_socket(e);
    c->get_connection_manager()->icontext_delete_socket(e);
    c->get_congestion_control()->icontext_delete_socket(e);

    map_.erase(s);
    assert(map_.find(s) == map_.end());
}

void TCPTahoe::icontext_set_socket_option(SetSocketOptionEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_set_socket_option(e);
    c->get_connection_manager()->icontext_set_socket_option(e);
    c->get_congestion_control()->icontext_set_socket_option(e);
}

void TCPTahoe::icontext_get_socket_option(GetSocketOptionEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;

    c->get_reliability()->icontext_get_socket_option(e);
    c->get_connection_manager()->icontext_get_socket_option(e);
    c->get_congestion_control()->icontext_get_socket_option(e);
}

bool TCPTahoe::is_room_in_send_buffer(SendEvent* e) {
    Socket* s = e->get_socket();
    int num_bytes_to_send = e->data_length();
    return s->get_send_buffer().size() + num_bytes_to_send <= MAX_BUFFER_SIZE;
}

void TCPTahoe::save_in_buffer_and_send_events(SendEvent* e) {
    int num_bytes_to_send = e->data_length();
    const char* data = (const char*) e->get_data();

    Socket* s = e->get_socket();
    s->get_send_buffer().append(data, num_bytes_to_send);

    ResponseEvent* response = new ResponseEvent(s, e->get_name(), e->get_map()[FILE_STRING]);
    response->put(RETURN_VALUE_STRING, Utils::itoa(num_bytes_to_send));
    response->put(ERRNO, Utils::itoa(0));

    dispatch(response);
    dispatch(new SendBufferNotEmptyEvent(s));
}

void TCPTahoe::create_and_dispatch_received_data(ReceiveEvent* e) {
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
    dispatch(new ReceiveBufferNotFullEvent(s));
}