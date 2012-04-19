#include "ippacket.h"

// the IP header has the following fields:
//
// name     size    type           definition
// ===================================================================
// version  4 bits    u_int8_t     4 (IPv4) or 6 (IPv6)
// ihl      4 bits    u_int8_t     header length in 32-bit words
// tos      8 bits    u_int8_t     type of service bits
// tot_len  16 bits   u_int16_t    length of the datagram in bytes
// id       16 bits   u_int16_t    fragmentation id
// frag_off 16 bits   u_int16_t    fragmentation offset
// ttl      8 bits    u_int8_t     time to live
// protocol 8 bits    u_int8_t     protocol for next header
// check    16 bits   u_int16_t    checksum
// saddr    32 bits   u_int32_t    source address
// daddr    32 bits   u_int32_t    destination address
//
// NOTE: All 16 bit fields must use htons() and ntohs() to convert
// between host and network byte order. All 32-bit fields must use
// htonl() and nothl() to convert between host and network byte order.
//
// This note does not apply for the checksum field, because the
// checksum algorithm is byte-order agnostic.
//
// The ordering of the 4-bit and smaller fields is handled by Linux
// libraries.

// public functions

IPPacket::IPPacket(struct nfq_q_handle* qh, int id, int payload_length,
                   unsigned char* payload, Filter filter,
                   bool preserve_payload) :
        Packet(qh, id, payload_length, payload, filter, preserve_payload) {

    ip_ = (struct iphdr*)payload_;
}

IPPacket::IPPacket(Packet& p, bool preserve_payload) :
        Packet(p, preserve_payload) {

    ip_ = (struct iphdr*)payload_;
}

IPPacket::~IPPacket() {
}

struct iphdr* IPPacket::ip() {
    return ip_;
}

u_int8_t IPPacket::get_ip_version() {
    return ip_->version;
}

void IPPacket::set_ip_version(u_int8_t version) {
    ip_->version = version;
}

u_int8_t IPPacket::get_ip_length_bytes() {
    return get_ip_length() * NUM_WORDS;
}

u_int8_t IPPacket::get_ip_length() {
    return ip_->ihl;
}

void IPPacket::set_ip_length(u_int8_t ihl) {
    ip_->ihl = ihl;
}

u_int8_t IPPacket::get_ip_tos() {
    return ip_->tos;
}

void IPPacket::set_ip_tos(u_int8_t tos) {
    ip_->tos = tos;
}

u_int16_t IPPacket::get_ip_datagram_length() {
    return ntohs(ip_->tot_len);
}

void IPPacket::set_ip_datagram_length(u_int16_t length) {
    ip_->tot_len = htons(length);
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

string IPPacket::to_s() {
    stringstream s;
    s << this->Packet::to_s() << endl;
    s << "ip ";
    s << get_ip_source_address_s() << " ";
    s << get_ip_destination_address_s() << " ";
    s << (int)get_ip_protocol() << " ";
    s << get_ip_datagram_length() << " ";
    s << (int)get_ip_length_bytes() << " ";
    s << (int)get_ip_ttl();
    return s.str();
}

string IPPacket::to_s_format() {
    stringstream s;
    s << this->Packet::to_s_format() << endl;
    s << "# ip source destination protocol datagram_length ihl ttl";
    return s.str();
}

void IPPacket::recalculate_ip_checksum() {
    ip_->check = 0;
    // ip_->check = this->checksum((u_int16_t*)ip_, sizeof(struct iphdr));
    ip_->check = checksum((u_int16_t*)ip_, sizeof(struct iphdr));
}

unsigned char* IPPacket::get_next_header() {
    // get IP header length
    int ip_length = get_ip_length_bytes();

    // check if this packet has a shim header
    if (get_ip_protocol() != WIFU_PROTOCOL)
        return (unsigned char*)(payload_ + ip_length);

    // skip over WiFu and shim headers
    struct wifuhdr* wifu = (struct wifuhdr*)(payload_ + ip_length);
    return (unsigned char*)(payload_ + ip_length + WIFU_LENGTH + wifu->length);
}

void IPPacket::insert_shim(unsigned char* shim, u_int8_t shim_type,
                           u_int8_t shim_length) {

    // make room for shim
    make_room_for_shim(shim_length);
    
    // Check if there is enough room for the shim header in case the
    // caller forgot to make room
    int wifu_length = WIFU_LENGTH + shim_length;
    u_int16_t new_length = get_ip_datagram_length() + wifu_length;
    if (new_length > MAX_IP_LENGTH)
        return;
    
    // allocate new packet
    int mangle_length = payload_length_ + wifu_length;
    unsigned char mangle[BUF_SIZE];
        
    // modify IP header and copy to new packet
    int ip_length = get_ip_length_bytes();
    u_int8_t protocol = get_ip_protocol();
    set_ip_protocol(WIFU_PROTOCOL);
    set_ip_datagram_length(new_length);
    recalculate_ip_checksum();
    memcpy(mangle, ip_, ip_length);

    // create WiFu header and copy to new packet
    struct wifuhdr* wifu = (struct wifuhdr*)(mangle + ip_length);
    wifu->version = WIFU_VERSION;
    wifu->type = shim_type;
    wifu->protocol = protocol;
    wifu->length = shim_length;

    // copy shim header to new packet
    unsigned char* new_shim = (unsigned char*)(wifu + 1);
    memcpy(new_shim, shim, shim_length);

    // copy rest of original to new packet
    unsigned char* orig = payload_ + ip_length;
    unsigned char* data = mangle + ip_length + wifu_length;
    memcpy(data, orig, payload_length_ - ip_length);

    // copy back into payload
    memcpy(payload_, mangle, mangle_length);
    payload_length_ = mangle_length;

    // clean up pointers
    cleanup_shim();
}

void IPPacket::make_room_for_shim(int length) {
    // do nothing in default implementation
}

void IPPacket::cleanup_shim() {
    // do nothing in default implementation
}

struct wifuhdr* IPPacket::get_wifu() {
    // check if this packet has a WiFu header
    if (get_ip_protocol() != WIFU_PROTOCOL)
        return 0;

    // get WiFu header and shim length
    int ip_length = get_ip_length_bytes();
    struct wifuhdr* wifu = (struct wifuhdr*)(payload_ + ip_length);
    return wifu;
}

unsigned char* IPPacket::get_shim() {
    // check if this packet has a WiFu header
    if (get_ip_protocol() != WIFU_PROTOCOL)
        return 0;

    // get WiFu header and shim length
    int ip_length = get_ip_length_bytes();
    struct wifuhdr* wifu = (struct wifuhdr*)(payload_ + ip_length);
    unsigned char* shim = (unsigned char*)(wifu + 1);
    return shim;
}

void IPPacket::remove_shim() {
    // check if this packet has a WiFu header
    if (get_ip_protocol() != WIFU_PROTOCOL)
        return;

    // get WiFu header and shim length
    int ip_length = get_ip_length_bytes();
    struct wifuhdr* wifu = (struct wifuhdr*)(payload_ + ip_length);
    u_int8_t shim_length = wifu->length;

    // modify IP header
    unsigned char mangle[BUF_SIZE];
    int wifu_length = WIFU_LENGTH + shim_length;
    u_int16_t new_length = get_ip_datagram_length() - wifu_length;
    set_ip_protocol(wifu->protocol);
    set_ip_datagram_length(new_length);
    recalculate_ip_checksum();
    memcpy(mangle, ip_, ip_length);

    // skip wifu header and copy rest of original to new packet
    int mangle_length = payload_length_ - wifu_length;
    unsigned char* orig = payload_ + ip_length + wifu_length;
    unsigned char* data = mangle + ip_length;
    memcpy(data, orig, mangle_length - ip_length);

    // copy back to payload
    memcpy(payload_, mangle, mangle_length);
    payload_length_ = mangle_length;

    // clean up pointers
    cleanup_shim();
}

u_int16_t IPPacket::checksum(u_int16_t* ptr, u_int16_t len) {
    long sum = 0;
    while (len > 1) {
        sum += *ptr++;
        if (sum & 0x80000000)
            sum = (sum & 0xffff) + (sum >> 16);
        len -= 2;
    }
    if (len > 0)
        sum += *((unsigned char*)ptr);
    while (sum >> 16)
        sum = (sum & 0xffff) + (sum >> 16);
    return ~sum;
}

// compute the UDP or TCP checksum
//
// header: pointer to a buffer that contains a UDP or TCP header plus data
u_int16_t IPPacket::udptcp_checksum(void* header) {

    // allocate a new packet
    unsigned char packet[BUF_SIZE];

    // get length of IP payload
    int length = get_payload_length() - get_ip_length_bytes();

    // setup IP pseudo header
    struct ip_pseudo_header* pseudo = (struct ip_pseudo_header*)packet;
    pseudo->saddr = ip_->saddr;
    pseudo->daddr = ip_->daddr;
    pseudo->zero = 0;
    pseudo->protocol = ip_->protocol;
    pseudo->tot_len = htons(length);

    // copy in the UDP or TCP header and the data
    u_int16_t packet_length = sizeof(struct ip_pseudo_header) + length;
    memcpy(packet + sizeof(struct ip_pseudo_header), header, length);

    // compute the UDP or TCP checksum
    u_int16_t sum = IPPacket::checksum((u_int16_t*)packet, packet_length);

    return sum;
}
