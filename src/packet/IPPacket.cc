#include "packet/IPPacket.h"

IPPacket::IPPacket() : length_set_(false) {
    ip_ = (struct iphdr*) payload_;

    set_ip_version(4);
    set_ip_header_length_words(sizeof(struct iphdr) / 4);
    set_ip_tos(0);
    set_ip_fragmentation_offset(0);
    set_ip_ttl(MAX_TTL);
}

IPPacket::IPPacket(IPPacket& p) : length_set_(false) {
    ip_ = (struct iphdr*) payload_;

    memcpy(payload_, p.payload_, p.get_ip_tot_length());
}

IPPacket::IPPacket(unsigned char* buffer, int length) : length_set_(false) {
    ip_ = (struct iphdr*) payload_;

    memcpy(payload_, buffer, length);
}

IPPacket::~IPPacket() {
}

unsigned char* IPPacket::get_payload() {
    return payload_;
}

unsigned char* IPPacket::get_data() {
    return get_next_header();
}

int IPPacket::get_data_length_bytes() {
    return get_ip_tot_length() - get_ip_header_length_bytes();
}

void IPPacket::set_data(unsigned char* data, int length) {
    memcpy(get_data(), data, length);
    set_ip_tot_length(get_ip_header_length_bytes() + length);
}

unsigned char* IPPacket::get_next_header() {
    return get_payload() + get_ip_header_length_bytes();
}

u_int8_t IPPacket::get_ip_version() {
    return ip_->version;
}

void IPPacket::set_ip_version(u_int8_t version) {
    ip_->version = version;
}

u_int8_t IPPacket::get_ip_header_length_bytes() {
    return get_ip_header_length_words() * 4;
}

u_int8_t IPPacket::get_ip_header_length_words() {
    return ip_->ihl;
}

void IPPacket::set_ip_header_length_words(u_int8_t ihl) {
    ip_->ihl = ihl;
}

u_int8_t IPPacket::get_ip_tos() {
    return ip_->tos;
}

void IPPacket::set_ip_tos(u_int8_t tos) {
    ip_->tos = tos;
}

u_int16_t IPPacket::get_ip_tot_length() {
    return ntohs(ip_->tot_len);
}

void IPPacket::set_ip_tot_length(u_int16_t length) {
    ip_->tot_len = htons(length);
    length_set_ = true;
}

u_int16_t IPPacket::get_ip_identifier() {
    return ntohs(ip_->id);
}

void IPPacket::set_ip_identifier(u_int16_t identifier) {
    ip_->id = htons(identifier);
}

u_int16_t IPPacket::get_ip_fragmentation_offset() {
    return ntohs(ip_->frag_off);
}

void IPPacket::set_ip_fragmentation_offset(u_int16_t frag_off) {
    ip_->frag_off = htons(frag_off);
}

u_int8_t IPPacket::get_ip_ttl() {
    return ip_->ttl;
}

void IPPacket::set_ip_ttl(u_int8_t ttl) {
    ip_->ttl = ttl;
}

u_int8_t IPPacket::get_ip_protocol() {
    return ip_->protocol;
}

void IPPacket::set_ip_protocol(u_int8_t protocol) {
    ip_->protocol = protocol;
}

u_int16_t IPPacket::get_ip_checksum() {
    return ip_->check;
}

void IPPacket::set_ip_checksum(u_int16_t checksum) {
    ip_->check = checksum;
}

u_int32_t IPPacket::get_ip_source_address() {
    return ntohl(ip_->saddr);
}

void IPPacket::set_ip_source_address(u_int32_t saddr) {
    ip_->saddr = htonl(saddr);
}

string IPPacket::get_ip_source_address_s() {
    char ip_addr_source[INET_ADDRSTRLEN];
    u_int32_t saddr = ip_->saddr;
    inet_ntop(AF_INET, &saddr, ip_addr_source, INET_ADDRSTRLEN);
    return string(ip_addr_source);
}

void IPPacket::set_ip_source_address_s(string saddr) {
    inet_pton(AF_INET, saddr.c_str(), &ip_->saddr);
}

u_int32_t IPPacket::get_ip_destination_address() {
    return ntohl(ip_->daddr);
}

void IPPacket::set_ip_destination_address(u_int32_t daddr) {
    ip_->daddr = htonl(daddr);
}

string IPPacket::get_ip_destination_address_s() {
    char ip_addr_dest[INET_ADDRSTRLEN];
    u_int32_t daddr = ip_->daddr;
    inet_ntop(AF_INET, &daddr, ip_addr_dest, INET_ADDRSTRLEN);
    return string(ip_addr_dest);
}

void IPPacket::set_ip_destination_address_s(string daddr) {
    inet_pton(AF_INET, daddr.c_str(), &ip_->daddr);
}

void IPPacket::init() {
    
}

bool IPPacket::length_is_set() {
    return length_set_;
}

int IPPacket::max_data_length() {
    return MTU - get_ip_header_length_bytes();
}
