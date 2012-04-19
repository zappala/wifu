#include "tcppacket.h"

// the TCP header has the following fields:
//
// name     size    type           definition
// ===================================================================
// source   16 bits   u_int16_t    source port
// dest     16 bits   u_int16_t    destination port
// seq      32 bits   u_int32_t    sequence number
// ack_seq  32 bits   u_int32_t    acknowledgement number
// doff     4 bits    u_int8_t     header length in 32-bite words
// res1     4 bits    u_int8_t     reserved (unused)
// res2     2 bits    u_int8_t     reserved (unused, ECE)
// urg      1 bit     bool         urgent bit
// ack      1 bit     bool         ACK bit
// psh      1 bit     bool         PSH bit
// rst      1 bit     bool         RST bit
// syn      1 bit     bool         SYN bit
// fin      1 bit     bool         FIN bit
// window   16 bits   u_int16_t    receive window size
// check    16 bits   u_int16_t    checksum
// urg_ptr  16 bits   u_int16_t    urgent data pointer

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

// public methods

TCPPacket::TCPPacket(struct nfq_q_handle* qh, int id, int payload_length,
                     unsigned char* payload, Filter filter,
                     bool preserve_payload) :
        IPPacket(qh, id, payload_length, payload, filter, preserve_payload) {
    
    tcp_ = (struct tcphdr*)get_next_header();
}

TCPPacket::TCPPacket(Packet& p, bool preserve_payload) :
        IPPacket(p, preserve_payload) {
    
    tcp_ = (struct tcphdr*)get_next_header();
}

TCPPacket::~TCPPacket() {
}

struct tcphdr* TCPPacket::tcp() {
    return tcp_;
}

u_int16_t TCPPacket::get_tcp_source_port() {
    return ntohs(tcp_->source);
}

void TCPPacket::set_tcp_source_port(u_int16_t port) {
    tcp_->source = htons(port);
}

u_int16_t TCPPacket::get_tcp_destination_port() {
    return ntohs(tcp_->dest);
}

void TCPPacket::set_tcp_destination_port(u_int16_t port) {
    tcp_->dest = htons(port);
}

u_int32_t TCPPacket::get_tcp_sequence_number() {
    return ntohl(tcp_->seq);
}

void TCPPacket::set_tcp_sequence_number(u_int32_t seq_num) {
    tcp_->seq = htonl(seq_num);
}

u_int32_t TCPPacket::get_tcp_ack_number() {
    return ntohl(tcp_->ack_seq);
}

void TCPPacket::set_tcp_ack_number(u_int32_t ack_num) {
    tcp_->ack_seq = htonl(ack_num);
}

int TCPPacket::get_tcp_length_bytes() {
    return get_tcp_length() * NUM_WORDS;
}

u_int16_t TCPPacket::get_tcp_length() {
    return tcp_->doff;
}

void TCPPacket::set_tcp_length(u_int16_t length) {
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
    return ntohs(tcp_->window);
}

void TCPPacket::set_tcp_receive_window_size(u_int16_t window) {
    tcp_->window = htons(window);
}

u_int16_t TCPPacket::get_tcp_checksum() {
    return tcp_->check;
}

void TCPPacket::set_tcp_checksum(u_int16_t checksum) {
    tcp_->check = checksum;
}

u_int16_t TCPPacket::get_tcp_urgent_pointer() {
    return ntohs(tcp_->urg_ptr);
}

void TCPPacket::set_tcp_urgent_pointer(u_int16_t urg_ptr) {
    tcp_->urg_ptr = htons(urg_ptr);
}

string TCPPacket::to_s() {
    stringstream s;
    s << this->IPPacket::to_s() << endl;
    s << "tcp ";
    s << get_tcp_source_port() << " ";
    s << get_tcp_destination_port() << " ";
    s << get_tcp_sequence_number() << " ";
    s << get_tcp_ack_number() << " ";
    s << get_tcp_length() << " ";
    s << is_tcp_urg() << " ";
    s << is_tcp_ack() << " ";
    s << is_tcp_psh() << " ";
    s << is_tcp_rst() << " ";
    s << is_tcp_syn() << " ";
    s << is_tcp_fin();
    return s.str();
}

string TCPPacket::to_s_format() {
    stringstream s;
    s << this->IPPacket::to_s_format() << endl;
    s << "# tcp sport dport seq_num ack_num header_length URG ACK PSH RST SYN FIN";
    return s.str();
}

void TCPPacket::recalculate_tcp_checksum() {
    tcp_->check = 0;
    tcp_->check = udptcp_checksum(tcp_);
}

// Adjust TCP MAXSEG option on SYN segments to enable room for shim header
void TCPPacket::make_room_for_shim(int length) {
    if (is_tcp_syn()) {
        set_tcp_maxseg_option(WIFU_LENGTH + length);
        recalculate_tcp_checksum();
    }
}

void TCPPacket::cleanup_shim() {
    tcp_ = (struct tcphdr*)get_next_header();
}

// set the MAXSEG TCP option -- should only be called for SYN segments
//
// extra: the amount of extra space needed -- the requested MSS will
//        be reduced by this amount
void TCPPacket::set_tcp_maxseg_option(u_int16_t extra) {
    if (!is_tcp_syn())
        return;
    int length = get_tcp_length_bytes() - sizeof(struct tcphdr);
    unsigned char* ptr = (unsigned char *)(tcp_ + 1);
    while (length > 0) {
        int opcode = *ptr++;
        int opsize;
        switch (opcode) {
            case TCPOPT_EOL:
                return;
            case TCPOPT_NOP:
                length--;
                continue;
            default:
                opsize = *ptr++;
                // "silly options"
                if (opsize < 2)
                    return;
                // don't parse partial options
                if (opsize > length)
                    return;
                switch (opcode) {
                    case TCPOPT_MAXSEG:
                        if (opsize == TCPOLEN_MAXSEG) {
                            struct tcp_maxseg_option* maxseg;
                            maxseg = (struct tcp_maxseg_option*)ptr;
                            maxseg->mss = htons(ntohs(maxseg->mss) - extra);
                        }
                        return;
                    case TCPOPT_WINDOW:
                    case TCPOPT_TIMESTAMP:
                    case TCPOPT_SACK_PERMITTED:
                    case TCPOPT_SACK:
                        break;
                }
                ptr += opsize-2;
                length -= opsize;
        }
    }
}

// ts_option: pointer to a TCP timestamp option structure (to be
// filled by this method)
void TCPPacket::get_tcp_timestamp_option(struct tcp_timestamp_option* ts_option) {
    int length = get_tcp_length_bytes() - sizeof(struct tcphdr);
    unsigned char* ptr = (unsigned char *)(tcp_ + 1);
    while (length > 0) {
        int opcode = *ptr++;
        int opsize;
        switch (opcode) {
            case TCPOPT_EOL:
                return;
            case TCPOPT_NOP:
                length--;
                continue;
            default:
                opsize = *ptr++;
                // "silly options"
                if (opsize < 2)
                    return;
                // don't parse partial options
                if (opsize > length)
                    return;
                switch (opcode) {
                    case TCPOPT_TIMESTAMP:
                        if (opsize == TCPOLEN_TIMESTAMP) {
                            struct tcp_timestamp_option* timestamp;
                            timestamp = (struct tcp_timestamp_option*) ptr;
                            // copy out the appropriate values
                            ts_option->TSval = timestamp->TSval;
                            ts_option->TSecr = timestamp->TSecr;
                        }
                        return;
                    case TCPOPT_MAXSEG:
                    case TCPOPT_WINDOW:
                    case TCPOPT_SACK_PERMITTED:
                    case TCPOPT_SACK:
                        break;
                }
                ptr += opsize-2;
                length -= opsize;
        }
    }
}
