#include "../headers/packet/WiFuPacket.h"

WiFuPacket::WiFuPacket() : IPPacket() {
    ports_ = (struct wifu_packet_header*) get_next_header();
}

WiFuPacket::WiFuPacket(IPPacket& p) : IPPacket(p) {
    ports_ = (struct wifu_packet_header*) get_next_header();
}

WiFuPacket::WiFuPacket(unsigned char* buffer, int length) : IPPacket(buffer, length) {
    ports_ = (struct wifu_packet_header*) get_next_header();
}

WiFuPacket::~WiFuPacket() {

}

u_int16_t WiFuPacket::get_source_port() {
    return ports_->sport;
}

void WiFuPacket::set_source_port(u_int16_t port) {
    ports_->sport = port;
}

u_int16_t WiFuPacket::get_destination_port() {
    return ports_->dport;
}

void WiFuPacket::set_destination_port(u_int16_t port) {
    ports_->dport = port;
}

AddressPort* WiFuPacket::get_source_address_port() {
    string addr = get_ip_source_address_s();
    return new AddressPort(addr, get_source_port());
}

AddressPort* WiFuPacket::get_dest_address_port() {
    string addr = get_ip_destination_address_s();
    return new AddressPort(addr, get_destination_port());
}