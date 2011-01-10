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
        
    }

    u_int32_t get_tcp_sequence_number() {
        return get_tcp_header()->seq;
    }

    void set_tcp_sequence_number(u_int32_t seq_num) {
        get_tcp_header()->seq = seq_num;
    }

    u_int32_t get_tcp_ack_number() {
        return get_tcp_header()->ack_seq;
    }

    void set_tcp_ack_number(u_int32_t ack_num) {
        get_tcp_header()->ack_seq = ack_num;
    }

    int get_tcp_header_length_bytes() {
        return get_tcp_header_length() * 4;
    }

    u_int16_t get_tcp_header_length() {
        // TODO: Remove this next line once we figure out the best way to cast a Packet to a TCPPacket.
        // Copy Constructor?
        // init() method?
        set_tcp_header_length(TCP_HEADER_SIZE / 4);
        return get_tcp_header()->doff;
    }

    void set_tcp_header_length(u_int16_t length) {
        get_tcp_header()->doff = length;
    }

    bool is_tcp_urg() {
        return get_tcp_header()->urg;
    }

    void set_tcp_urg(bool urg) {
        get_tcp_header()->urg = urg;
    }

    bool is_tcp_ack() {
        return get_tcp_header()->ack;
    }

    void set_tcp_ack(bool ack) {
        get_tcp_header()->ack = ack;
    }

    bool is_tcp_psh() {
        return get_tcp_header()->psh;
    }

    void set_tcp_psh(bool psh) {
        get_tcp_header()->psh = psh;
    }

    bool is_tcp_rst() {
        return get_tcp_header()->rst;
    }

    void set_tcp_rst(bool rst) {
        get_tcp_header()->rst = rst;
    }

    bool is_tcp_syn() {
        return get_tcp_header()->syn;
    }

    void set_tcp_syn(bool syn) {
        get_tcp_header()->syn = syn;
    }

    bool is_tcp_fin() {
        return get_tcp_header()->fin;
    }

    void set_tcp_fin(bool fin) {
        get_tcp_header()->fin = fin;
    }

    u_int16_t get_tcp_receive_window_size() {
        return get_tcp_header()->window;
    }

    void set_tcp_receive_window_size(u_int16_t window) {
        get_tcp_header()->window = window;
    }

    u_int16_t get_tcp_checksum() {
        return get_tcp_header()->check;
    }

    void set_tcp_checksum(u_int16_t checksum) {
        get_tcp_header()->check = checksum;
    }

    u_int16_t get_tcp_urgent_pointer() {
        return get_tcp_header()->urg_ptr;
    }

    void set_tcp_urgent_pointer(u_int16_t urg_ptr) {
        get_tcp_header()->urg_ptr = urg_ptr;
    }

private:
    struct tcp_header* get_tcp_header() {
        return (struct tcp_header*) get_header();
    }


};

#endif	/* _TCPPACKET_H */

