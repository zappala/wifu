/* 
 * File:   UDPPacket.cc
 * Author: leer
 * 
 * Created on June 22, 2011, 10:48 AM
 */

#include "UDPPacket.h"

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

unsigned char* UDPPacket::get_data()
{
    return get_next_header() + UDP_HEADER_LENGTH_BYTES;
}

int UDPPacket::get_length_bytes() {
    return get_ip_tot_length() - get_ip_header_length_bytes();//the following would be the data length: - UDP_HEADER_LENGTH_BYTES;
}

void UDPPacket::set_data(unsigned char* data, int length) {
    if (data_set_) {
        // Can not set the data twice
        throw IllegalStateException();
    }

    memcpy(get_data(), data, length);
    data_set_ = true;

    u_int8_t udp_len = UDP_HEADER_LENGTH_BYTES;
    u_int8_t ip_len = get_ip_header_length_bytes();
    set_ip_tot_length(ip_len + udp_len + length);
}

int UDPPacket::max_data_length() {
    return IPPacket::max_data_length() - UDP_HEADER_LENGTH_BYTES;
}

u_int16_t UDPPacket::get_udp_checksum() const {
    return udp_->check;
}

void UDPPacket::set_udp_checksum(u_int16_t checksum) {
    cout << "UDPPacket::set_udp_checksum(): UDP struct is " << udp_ << endl;
    udp_->check = checksum;
}

string UDPPacket::to_s() const {
    stringstream s;
    s << IPPacket::to_s() << endl
            << "udp "
            << get_source_port() << " "
            << get_destination_port() << " ";
    return s.str();
}

string UDPPacket::to_s_format() const {
    stringstream s;
    s << IPPacket::to_s_format() << endl
      << "# UDP sport dport seq_num ack_num header_length URG ACK PSH RST SYN FIN rcv_wnd";
    return s.str();
}

bool UDPPacket::operator ==(const IPPacket& other) const {
    //    cout << "UDPPacket::operator ==()" << endl;
    if (!WiFuPacket::operator ==(other)) {
        return false;
    }

    UDPPacket const* other_ptr = dynamic_cast<UDPPacket const*> (&other);

    bool equal = other_ptr != NULL;
    equal = equal && udp_->check == other_ptr->udp_->check;
    equal = equal && udp_->dest == other_ptr->udp_->dest;
    equal = equal && udp_->source == other_ptr->udp_->source;
    // TODO: we cannot guarantee what the receive window will be on tests
    //equal = equal && udp_->window == other_ptr->udp_->window;

    return equal;
}

bool UDPPacket::operator !=(const IPPacket& other) const {
    //    cout << "UDPPacket::operator !=()" << endl;
    return !(*this == other);
}
/*
 * void UDPPacket::init() {
    udp_ = (struct UDPhdr*) get_next_header();
    set_udp_data_offset(sizeof (struct UDPhdr) / 4);
    set_ip_tot_length(get_ip_header_length_bytes() + get_udp_header_length_bytes());
}*/