#include "udppacket.h"

// the UDP header has the following fields:
//
// name     size    type           definition
// ===================================================================
// source   16 bits   u_int16_t    source port
// dest     16 bits   u_int16_t    destination port
// len      16 bits   u_int16_t    length in bytes of header and data
// check    16 bits   u_int16_t    checksum

//
// NOTE: All 16 bit fields must use htons() and ntohs() to convert
// between host and network byte order.
//
// This note does not apply for the checksum field, because the
// checksum algorithm is byte-order agnostic.
//

// public methods

UDPPacket::UDPPacket(struct nfq_q_handle* qh, int id, int payload_length,
                     unsigned char* payload, Filter filter,
                     bool preserve_payload) :
        IPPacket(qh, id, payload_length, payload, filter, preserve_payload) {
    
    udp_ = (struct udphdr*)get_next_header();
}

UDPPacket::UDPPacket(Packet& p, bool preserve_payload) :
        IPPacket(p, preserve_payload) {
    
    udp_ = (struct udphdr*)get_next_header();
}

UDPPacket::~UDPPacket() {
}

struct udphdr* UDPPacket::udp() {
    return udp_;
}

u_int16_t UDPPacket::get_udp_source_port() {
    return ntohs(udp_->source);
}

void UDPPacket::set_udp_source_port(u_int16_t port) {
    udp_->source = htons(port);
}

u_int16_t UDPPacket::get_udp_destination_port() {
    return ntohs(udp_->dest);
}

void UDPPacket::set_udp_destination_port(u_int16_t port) {
    udp_->dest = htons(port);
}

u_int16_t UDPPacket::get_udp_length() {
    return ntohs(udp_->len);
}

void UDPPacket::set_udp_length(u_int16_t length) {
    udp_->len = htons(length);
}

u_int16_t UDPPacket::get_udp_checksum() {
    return udp_->check;
}

void UDPPacket::set_udp_checksum(u_int16_t checksum) {
    udp_->check = checksum;
}

string UDPPacket::to_s() {
    stringstream s;
    s << this->IPPacket::to_s() << endl;
    s << "udp ";
    s << get_udp_source_port() << " ";
    s << get_udp_destination_port() << " ";
    s << get_udp_length();
    return s.str();
}

string UDPPacket::to_s_format() {
    stringstream s;
    s << this->IPPacket::to_s_format() << endl;
    s << "# udp sport dport length";
    return s.str();
}

void UDPPacket::recalculate_udp_checksum() {
    udp_->check = 0;
    udp_->check = udptcp_checksum(udp_);
}

void UDPPacket::make_room_for_shim(u_int8_t length) {
    // do nothing -- application must send packets small enough for the
    // shim to fit
}

void UDPPacket::cleanup_shim() {
    udp_ = (struct udphdr*)get_next_header();
}
