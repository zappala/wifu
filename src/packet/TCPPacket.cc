#include <netinet/tcp.h>

#include "packet/TCPPacket.h"
#include "packet/WiFuPacket.h"

TCPPacket::TCPPacket() : WiFuPacket() {
    init();
}

TCPPacket::TCPPacket(IPPacket& p) : WiFuPacket(p) {
    init();
}

TCPPacket::TCPPacket(unsigned char* buffer, int length) : WiFuPacket(buffer, length) {
    init();
}

TCPPacket::~TCPPacket() {

}

unsigned char* TCPPacket::get_data() {
    return get_next_header() + sizeof (struct tcphdr);
}

int TCPPacket::get_data_length_bytes() {
    return get_ip_datagram_length() - get_ip_length_bytes() - sizeof (struct tcphdr);
}

void TCPPacket::set_data(unsigned char* data, int length) {
    memcpy(get_data(), data, length);
    set_ip_datagram_length(get_ip_length_bytes() + sizeof (struct tcphdr) + length);
}

u_int32_t TCPPacket::get_tcp_sequence_number() {
    return tcp_->seq;
}

void TCPPacket::set_tcp_sequence_number(u_int32_t seq_num) {
    tcp_->seq = seq_num;
}

u_int32_t TCPPacket::get_tcp_ack_number() {
    return tcp_->ack_seq;
}

void TCPPacket::set_tcp_ack_number(u_int32_t ack_num) {
    tcp_->ack_seq = ack_num;
}

int TCPPacket::get_tcp_header_length_bytes() {
    return get_tcp_header_length() * 4;
}

u_int16_t TCPPacket::get_tcp_header_length() {
    return tcp_->doff;
}

void TCPPacket::set_tcp_header_length(u_int16_t length) {
    tcp_->doff = length;
}

bool TCPPacket::is_tcp_urg() {
    return tcp_->urg;
}

void TCPPacket::set_tcp_urg(bool urg) {
    tcp_->urg = urg;
}

bool TCPPacket::is_tcp_ack() {
    return tcp_->ack;
}

void TCPPacket::set_tcp_ack(bool ack) {
    tcp_->ack = ack;
}

bool TCPPacket::is_tcp_psh() {
    return tcp_->psh;
}

void TCPPacket::set_tcp_psh(bool psh) {
    tcp_->psh = psh;
}

bool TCPPacket::is_tcp_rst() {
    return tcp_->rst;
}

void TCPPacket::set_tcp_rst(bool rst) {
    tcp_->rst = rst;
}

bool TCPPacket::is_tcp_syn() {
    return tcp_->syn;
}

void TCPPacket::set_tcp_syn(bool syn) {
    tcp_->syn = syn;
}

bool TCPPacket::is_tcp_fin() {
    return tcp_->fin;
}

void TCPPacket::set_tcp_fin(bool fin) {
    tcp_->fin = fin;
}

u_int16_t TCPPacket::get_tcp_receive_window_size() {
    return tcp_->window;
}

void TCPPacket::set_tcp_receive_window_size(u_int16_t window) {
    tcp_->window = window;
}

u_int16_t TCPPacket::get_tcp_checksum() {
    return tcp_->check;
}

void TCPPacket::set_tcp_checksum(u_int16_t checksum) {
    tcp_->check = checksum;
}

u_int16_t TCPPacket::get_tcp_urgent_pointer() {
    return tcp_->urg_ptr;
}

void TCPPacket::set_tcp_urgent_pointer(u_int16_t urg_ptr) {
    tcp_->urg_ptr = urg_ptr;
}

void TCPPacket::init() {
    tcp_ = (struct tcphdr*) get_next_header();
    set_tcp_header_length(sizeof (struct tcphdr) / 4);
}

bool TCPPacket::is_naked_ack() {
    if (is_tcp_ack() &&
        get_data_length_bytes() == 0 &&
        !is_tcp_fin() &&
        !is_tcp_psh() &&
        !is_tcp_rst() &&
        !is_tcp_syn() &&
        !is_tcp_urg()) {
        return true;
    }
    return false;
}
