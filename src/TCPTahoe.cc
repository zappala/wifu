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

}

void TCPTahoe::icontext_bind(BindEvent* e) {

}

void TCPTahoe::icontext_listen(ListenEvent* e) {

}

void TCPTahoe::icontext_receive_packet(NetworkReceivePacketEvent* e) {

}

void TCPTahoe::icontext_send_packet(SendPacketEvent* e) {

}

void TCPTahoe::icontext_connect(ConnectEvent* e) {

}

void TCPTahoe::icontext_accept(AcceptEvent* e) {

}

void TCPTahoe::icontext_new_connection_established(ConnectionEstablishedEvent* e) {

}

void TCPTahoe::icontext_new_conneciton_initiated(ConnectionInitiatedEvent* e) {

}

void TCPTahoe::icontext_close(CloseEvent* e) {

}

void TCPTahoe::icontext_timer_fired_event(TimerFiredEvent* e) {

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
}

bool TCPTahoe::icontext_can_receive(Socket* s) {
}

void TCPTahoe::icontext_delete_socket(DeleteSocketEvent* e) {
}

void TCPTahoe::icontext_set_socket_option(SetSocketOptionEvent* e) {
}

void TCPTahoe::icontext_get_socket_option(GetSocketOptionEvent* e) {
}