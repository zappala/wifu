#include "TCPTahoe.h"

TCPTahoe::TCPTahoe() : Protocol(TCP_TAHOE) {

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
    c->get_connection_manager()->icontext_socket(e);
}

void TCPTahoe::icontext_bind(BindEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;
    c->get_connection_manager()->icontext_bind(e);
}

void TCPTahoe::icontext_listen(ListenEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;
    c->get_connection_manager()->icontext_listen(e);
}

void TCPTahoe::icontext_receive_packet(NetworkReceivePacketEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;
    c->get_connection_manager()->icontext_receive_packet(e);
}

void TCPTahoe::icontext_send_packet(SendPacketEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;
    c->get_connection_manager()->icontext_send_packet(e);

    send_network_packet(e->get_socket(), e->get_packet());
}

void TCPTahoe::icontext_connect(ConnectEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;
    c->get_connection_manager()->icontext_connect(e);
}

void TCPTahoe::icontext_accept(AcceptEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;
    c->get_connection_manager()->icontext_accept(e);
}

void TCPTahoe::icontext_new_connection_established(ConnectionEstablishedEvent* e) {

}

void TCPTahoe::icontext_new_conneciton_initiated(ConnectionInitiatedEvent* e) {
    Socket* listening_socket = e->get_socket();
    Socket* new_socket = e->get_new_socket();

    TCPTahoeIContextContainer* listening_cc = map_.find(listening_socket)->second;
    map_[new_socket] = listening_cc;

    TCPTahoeIContextContainer* new_cc = new TCPTahoeIContextContainer();
    map_[listening_socket] = new_cc;

    // TODO: Move cache over?

    new_cc->get_connection_manager()->icontext_new_conneciton_initiated(e);
}

void TCPTahoe::icontext_close(CloseEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;
    c->get_connection_manager()->icontext_close(e);

    ResponseEvent* response = new ResponseEvent(s, e->get_name(), e->get_map()[FILE_STRING]);
    response->put(RETURN_VALUE_STRING, Utils::itoa(0));
    response->put(ERRNO, Utils::itoa(0));
    dispatch(response);
}

void TCPTahoe::icontext_timer_fired_event(TimerFiredEvent* e) {
    Socket* s = e->get_socket();
    TCPTahoeIContextContainer* c = map_.find(s)->second;
    c->get_connection_manager()->icontext_timer_fired_event(e);
}

void TCPTahoe::icontext_resend_packet(ResendPacketEvent* e) {

}

void TCPTahoe::icontext_send(SendEvent* e) {

}

void TCPTahoe::icontext_receive(ReceiveEvent* e) {

}

void TCPTahoe::icontext_receive_buffer_not_empty(ReceiveBufferNotEmptyEvent* e) {

}

void TCPTahoe::icontext_receive_buffer_not_full(ReceiveBufferNotFullEvent* e) {
}

void TCPTahoe::icontext_send_buffer_not_empty(SendBufferNotEmptyEvent* e) {
}

void TCPTahoe::icontext_send_buffer_not_full(SendBufferNotFullEvent* e) {
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
    c->get_connection_manager()->icontext_delete_socket(e);

    map_.erase(s);
    assert(map_.find(s) == map_.end());
}

void TCPTahoe::icontext_set_socket_option(SetSocketOptionEvent* e) {
}

void TCPTahoe::icontext_get_socket_option(GetSocketOptionEvent* e) {
}