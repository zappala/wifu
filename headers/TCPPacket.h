/* 
 * File:   TCPPacket.h
 * Author: rbuck
 *
 * Created on January 7, 2011, 2:42 PM
 */

#ifndef _TCPPACKET_H
#define	_TCPPACKET_H

#include "../headers/Packet.h"

/**
 * Mirror of tcpheader except for ports
 */
struct tcp_header {
    u_int32_t seq;
    u_int32_t ack_seq;
#if __BYTE_ORDER == __LITTLE_ENDIAN
    u_int16_t res1 : 4;
    u_int16_t doff : 4;
    u_int16_t fin : 1;
    u_int16_t syn : 1;
    u_int16_t rst : 1;
    u_int16_t psh : 1;
    u_int16_t ack : 1;
    u_int16_t urg : 1;
    u_int16_t res2 : 2;
#elif __BYTE_ORDER == __BIG_ENDIAN
    u_int16_t doff : 4;
    u_int16_t res1 : 4;
    u_int16_t res2 : 2;
    u_int16_t urg : 1;
    u_int16_t ack : 1;
    u_int16_t psh : 1;
    u_int16_t rst : 1;
    u_int16_t syn : 1;
    u_int16_t fin : 1;
#else
#error "Adjust your <bits/endian.h> defines"
#endif
    u_int16_t window;
    u_int16_t check;
    u_int16_t urg_ptr;

};

#define TCP_HEADER_SIZE sizeof(struct tcp_header)

class TCPPacket : public Packet {
public:

    TCPPacket(AddressPort* source,
            AddressPort* dest,
            unsigned char* data,
            int data_length)
    : Packet(source, dest, data, data_length, TCP_HEADER_SIZE) {
        tcp_ = (struct tcp_header*) get_header();
        set_tcp_header_length(TCP_HEADER_SIZE / 4);
    }

    u_int32_t get_tcp_sequence_number() {
        return tcp_->seq;
    }

    void set_tcp_sequence_number(u_int32_t seq_num) {
        tcp_->seq = seq_num;
    }

    u_int32_t get_tcp_ack_number() {
        return tcp_->ack_seq;
    }

    void set_tcp_ack_number(u_int32_t ack_num) {
        tcp_->ack_seq = ack_num;
    }

    int get_tcp_header_length_bytes() {
        return get_tcp_header_length() * 4;
    }

    u_int16_t get_tcp_header_length() {
        return tcp_->doff;
    }

    void set_tcp_header_length(u_int16_t length) {
        tcp_->doff = length;
    }

    bool is_tcp_urg() {
        return tcp_->urg;
    }

    void set_tcp_urg(bool urg) {
        tcp_->urg = urg;
    }

    bool is_tcp_ack() {
        return tcp_->ack;
    }

    void set_tcp_ack(bool ack) {
        tcp_->ack = ack;
    }

    bool is_tcp_psh() {
        return tcp_->psh;
    }

    void set_tcp_psh(bool psh) {
        tcp_->psh = psh;
    }

    bool is_tcp_rst() {
        return tcp_->rst;
    }

    void set_tcp_rst(bool rst) {
        tcp_->rst = rst;
    }

    bool is_tcp_syn() {
        return tcp_->syn;
    }

    void set_tcp_syn(bool syn) {
        tcp_->syn = syn;
    }

    bool is_tcp_fin() {
        return tcp_->fin;
    }

    void set_tcp_fin(bool fin) {
        tcp_->fin = fin;
    }

    u_int16_t get_tcp_receive_window_size() {
        return tcp_->window;
    }

    void set_tcp_receive_window_size(u_int16_t window) {
        tcp_->window = window;
    }

    u_int16_t get_tcp_checksum() {
        return tcp_->check;
    }

    void set_tcp_checksum(u_int16_t checksum) {
        tcp_->check = checksum;
    }

    u_int16_t get_tcp_urgent_pointer() {
        return tcp_->urg_ptr;
    }

    void set_tcp_urgent_pointer(u_int16_t urg_ptr) {
        tcp_->urg_ptr = urg_ptr;
    }

private:
    struct tcp_header* tcp_;


};

#endif	/* _TCPPACKET_H */

