#include "../headers/packet/WiFuPacket.h"
#include "AddressPort.h"

WiFuPacket::WiFuPacket() : IPPacket() {
    ports_ = (struct wifu_end_header*) (get_payload() + get_ip_length_bytes());
}

WiFuPacket::WiFuPacket(WiFuPacket const& p) : IPPacket(p) {
    ports_ = (struct wifu_end_header*) (get_payload() + get_ip_length_bytes());
}

WiFuPacket::WiFuPacket(unsigned char* buffer, int length) : WiFuPacket(buffer, length) {
    ports_ = (struct wifu_end_header*) (get_payload() + get_ip_length_bytes());
}

WiFuPacket::~WiFuPacket() {

}

u_int16_t WiFuPacket::get_source_port() {
    return ntohs(ports_->sport);
}

void WiFuPacket::set_source_port(u_int16_t port) {
    ports_->sport = htons(port);
}

u_int16_t WiFuPacket::get_destination_port() {
    return ntohs(ports_->dport);
}

void WiFuPacket::set_destination_port(u_int16_t port) {
    ports_->dport = htons(port);
}

AddressPort* WiFuPacket::get_source_address_port() {
    return new AddressPort(get_ip_source_address_s(), get_source_port());
}

AddressPort* WiFuPacket::get_dest_address_port() {
    return new AddressPort(get_ip_destination_address_s(), get_destination_port());
}