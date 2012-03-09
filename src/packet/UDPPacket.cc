/* 
 * File:   UDPPacket.cc
 * Author: leer
 * 
 * Created on June 22, 2011, 10:48 AM
 */

#include <netinet/udp.h>

#include "packet/UDPPacket.h"

UDPPacket::UDPPacket() : WiFuPacket(), data_set_(false){
    init();
}

UDPPacket::UDPPacket(const UDPPacket& orig) {
}

UDPPacket::~UDPPacket() {
}

void UDPPacket::init() {
    udp_ = (struct udphdr*) get_next_header();
    set_ip_tot_length(get_ip_header_length_bytes() + UDP_HEADER_LENGTH_BYTES);
}

u_int16_t UDPPacket::get_udp_length_bytes() const {
    return ntohs(udp_->len);
}

void UDPPacket::set_udp_length_bytes(u_int16_t newlen) {
    udp_->len = htons(newlen);
}

unsigned char* UDPPacket::get_data()
{
    return get_next_header() + UDP_HEADER_LENGTH_BYTES;
}

int UDPPacket::get_data_length_bytes() {
    return get_ip_tot_length() - get_ip_header_length_bytes() - UDP_HEADER_LENGTH_BYTES;
}

void UDPPacket::set_data(unsigned char* data, int length) {
    memcpy(get_data(), data, length);
    set_udp_length_bytes(UDP_HEADER_LENGTH_BYTES + length);
    set_ip_tot_length(get_ip_header_length_bytes() + UDP_HEADER_LENGTH_BYTES + length);
    calculate_and_set_udp_checksum();
}

int UDPPacket::max_data_length() {
    return IPPacket::max_data_length() - UDP_HEADER_LENGTH_BYTES;
}

u_int16_t UDPPacket::get_udp_checksum() const {
    return udp_->check;
}

void UDPPacket::set_udp_checksum(u_int16_t checksum) {
    udp_->check = checksum;
}

gcstring UDPPacket::to_s() const {
    char buffer[200];
    sprintf(buffer, "%s\nudp %hu %hu %hu %hu", IPPacket::to_s().c_str(), get_source_port(), get_destination_port(), get_udp_length_bytes(), get_udp_checksum());
    return buffer;
}

gcstring UDPPacket::to_s_format() const {
    char buffer[200];
    sprintf(buffer, "%s\n# udp sport dport len checksum", IPPacket::to_s_format().c_str());
    return buffer;
}

bool UDPPacket::operator ==(const IPPacket& other) const {
    if (!WiFuPacket::operator ==(other)) {
        return false;
    }

    UDPPacket const* other_ptr = dynamic_cast<UDPPacket const*> (&other);

    bool equal = other_ptr != NULL;
    equal = equal && udp_->check == other_ptr->udp_->check;
    equal = equal && udp_->dest == other_ptr->udp_->dest;
    equal = equal && udp_->source == other_ptr->udp_->source;
    equal = equal && udp_->len == other_ptr->udp_->len;
    // TODO: we cannot guarantee what the receive window will be on tests
    //equal = equal && udp_->window == other_ptr->udp_->window;

    return equal;
}

void UDPPacket::calculate_and_set_udp_checksum() {
    set_udp_checksum(0);
    set_udp_checksum(compute_next_checksum());
}

bool UDPPacket::is_valid_udp_checksum() {
    u_int16_t current_checksum = get_udp_checksum();
    calculate_and_set_udp_checksum();
    u_int16_t calculated_checksum = get_udp_checksum();
    set_udp_checksum(current_checksum);
    return current_checksum == calculated_checksum;
}

bool UDPPacket::operator !=(const IPPacket& other) const {
    return !(*this == other);
}