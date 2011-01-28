#include "../headers/packet/WiFuPacket.h"

WiFuPacket::WiFuPacket() : IPPacket() {
    init();
}

WiFuPacket::WiFuPacket(IPPacket& p) : IPPacket(p) {
    init();
}

WiFuPacket::WiFuPacket(unsigned char* buffer, int length) : IPPacket(buffer, length) {
    init();
}

WiFuPacket::~WiFuPacket() {

}

unsigned char* WiFuPacket::get_data() {
    return get_next_header() + sizeof(struct wifu_packet_header);
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
    return new AddressPort(get_ip_source_address_s().c_str(), get_source_port());
}

AddressPort* WiFuPacket::get_dest_address_port() {
    return new AddressPort(get_ip_destination_address_s().c_str(), get_destination_port());
}

void WiFuPacket::init() {
    ports_ = (struct wifu_packet_header*) get_next_header();
}