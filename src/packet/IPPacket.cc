#include "packet/IPPacket.h"

IPPacket::IPPacket() : length_set_(false) {
    init();
}

//IPPacket::IPPacket(IPPacket& p) : length_set_(false) {
//    ip_ = (struct iphdr*) payload_;
//
//    memcpy(payload_, p.payload_, p.get_ip_tot_length());
//}

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

void IPPacket::pack() {

}

unsigned char* IPPacket::get_next_header() {
    return get_payload() + get_ip_header_length_bytes();
}

u_int8_t IPPacket::get_ip_version() const {
    return ip_->version;
}

void IPPacket::set_ip_version(u_int8_t version) {
    ip_->version = version;
}

u_int8_t IPPacket::get_ip_header_length_bytes() const {
    return get_ip_header_length_words() * 4;
}

u_int8_t IPPacket::get_ip_header_length_words() const {
    return ip_->ihl;
}

void IPPacket::set_ip_header_length_words(u_int8_t ihl) {
    ip_->ihl = ihl;
}

u_int8_t IPPacket::get_ip_tos() const {
    return ip_->tos;
}

void IPPacket::set_ip_tos(u_int8_t tos) {
    ip_->tos = tos;
}

u_int16_t IPPacket::get_ip_tot_length() const {
    return ntohs(ip_->tot_len);
}

void IPPacket::set_ip_tot_length(u_int16_t length) {
    ip_->tot_len = htons(length);
    length_set_ = true;
}

u_int16_t IPPacket::get_ip_identifier() const {
    return ntohs(ip_->id);
}

void IPPacket::set_ip_identifier(u_int16_t identifier) {
    ip_->id = htons(identifier);
}

u_int16_t IPPacket::get_ip_fragmentation_offset() const {
    return ntohs(ip_->frag_off);
}

void IPPacket::set_ip_fragmentation_offset(u_int16_t frag_off) {
    ip_->frag_off = htons(frag_off);
}

u_int8_t IPPacket::get_ip_ttl() const {
    return ip_->ttl;
}

void IPPacket::set_ip_ttl(u_int8_t ttl) {
    ip_->ttl = ttl;
}

u_int8_t IPPacket::get_ip_protocol() const {
    return ip_->protocol;
}

void IPPacket::set_ip_protocol(u_int8_t protocol) {
    ip_->protocol = protocol;
}

u_int16_t IPPacket::get_ip_checksum() const {
    return ip_->check;
}

void IPPacket::set_ip_checksum(u_int16_t checksum) {
    ip_->check = checksum;
}

u_int32_t IPPacket::get_ip_source_address() const {
    return ntohl(ip_->saddr);
}

void IPPacket::set_ip_source_address(u_int32_t saddr) {
    ip_->saddr = htonl(saddr);
}

string IPPacket::get_ip_source_address_s() const {
    char ip_addr_source[INET_ADDRSTRLEN];
    u_int32_t saddr = ip_->saddr;
    inet_ntop(AF_INET, &saddr, ip_addr_source, INET_ADDRSTRLEN);
    return string(ip_addr_source);
}

void IPPacket::set_ip_source_address_s(string saddr) {
    inet_pton(AF_INET, saddr.c_str(), &ip_->saddr);
}

u_int32_t IPPacket::get_ip_destination_address() const {
    return ntohl(ip_->daddr);
}

void IPPacket::set_ip_destination_address(u_int32_t daddr) {
    ip_->daddr = htonl(daddr);
}

string IPPacket::get_ip_destination_address_s() const {
    char ip_addr_dest[INET_ADDRSTRLEN];
    u_int32_t daddr = ip_->daddr;
    inet_ntop(AF_INET, &daddr, ip_addr_dest, INET_ADDRSTRLEN);
    return string(ip_addr_dest);
}

void IPPacket::set_ip_destination_address_s(string daddr) {
    inet_pton(AF_INET, daddr.c_str(), &ip_->daddr);
}

u_int16_t IPPacket::checksum(u_int16_t* ptr, u_int16_t len) {
    long sum = 0;
    while (len > 1) {
        sum += *ptr++;
        if (sum & 0x80000000)
            sum = (sum & 0xffff) + (sum >> 16);
        len -= 2;
    }
    if (len > 0) {
        sum += *((unsigned char*) ptr);
    }
    while (sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }
    return ~sum;
}

void IPPacket::calculate_and_set_ip_checksum() {
    set_ip_checksum(0);
    set_ip_checksum(checksum((u_int16_t*) ip_, get_ip_header_length_bytes()));
}

bool IPPacket::is_valid_ip_checksum() {
    u_int16_t current_checksum = get_ip_checksum();
    calculate_and_set_ip_checksum();
    u_int16_t calculated_checksum = get_ip_checksum();
    set_ip_checksum(current_checksum);
    return current_checksum == calculated_checksum;
}

u_int16_t IPPacket::compute_next_checksum() {

    // We overwrite the current ip header with the pseudo-header,
    // then put it back after checksum calculation

    // save the current ip_header
    struct iphdr ip_header;
    memcpy(&ip_header, ip_, sizeof(ip_));

    unsigned char* header = IPPacket::get_next_header();
    header -= sizeof(struct ip_pseudo_header);

    // get length of IP payload
    int length = get_ip_tot_length() - get_ip_header_length_bytes();

    // setup IP pseudo header
    struct ip_pseudo_header* pseudo = (struct ip_pseudo_header*) header;
    pseudo->saddr = ip_header.saddr;
    pseudo->daddr = ip_header.daddr;
    pseudo->zero = 0;
    pseudo->protocol = ip_header.protocol;
    pseudo->tot_len = htons(length);

    u_int16_t packet_length = sizeof (struct ip_pseudo_header) + length;
    u_int16_t sum = IPPacket::checksum((u_int16_t*) header, packet_length);

    // put the ip header back
    memcpy(&ip_, &ip_header, sizeof(ip_));

    return sum;
}

void IPPacket::init() {
    ip_ = (struct iphdr*) payload_;

    memset(ip_, 0, sizeof (iphdr));

    set_ip_version(4);
    set_ip_header_length_words(sizeof (struct iphdr) / 4);
    //tos
    set_ip_tot_length(get_ip_header_length_bytes());
    //id
    //flags
    //fragmentation offset
    set_ip_ttl(MAX_TTL);
    //protocol
    //checksum
    //source
    //destination
}

bool IPPacket::length_is_set() const {
    return length_set_;
}

int IPPacket::max_data_length() const {
    return MTU - get_ip_header_length_bytes();
}

string IPPacket::to_s() const {
    stringstream s;
    s << "ip "
            << get_ip_source_address_s() << " "
            << get_ip_destination_address_s() << " "
            << (int) get_ip_protocol() << " "
            << get_ip_tot_length() << " "
            << (int) get_ip_header_length_bytes() << " "
            << (int) get_ip_ttl();
    return s.str();
}

string IPPacket::to_s_format() const {
    stringstream s;
    s << "# ip source destination protocol datagram_length ihl ttl";
    return s.str();
}

bool IPPacket::operator ==(const IPPacket& other) const {
    //cout << "IPPacket::operator ==()" << endl;
    return !memcmp(payload_, other.payload_, sizeof (struct iphdr));
}

bool IPPacket::operator !=(const IPPacket& other) const {
    //cout << "IPPacket::operator !=()" << endl;
    return !(*this == other);
}

ostream & operator <<(ostream& os, const IPPacket& packet) {
    return os << packet.to_s_format() << endl << packet.to_s();
}
