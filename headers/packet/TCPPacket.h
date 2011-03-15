/* 
 * File:   TCPPacket.h
 * Author: rbuck
 *
 * Created on January 7, 2011, 2:42 PM
 */

#ifndef _TCPPACKET_H
#define	_TCPPACKET_H

#include "WiFuPacket.h"
#include <netinet/tcp.h>


class TCPPacket : public WiFuPacket {
public:

    TCPPacket();
    TCPPacket(IPPacket&);
    TCPPacket(unsigned char* buffer, int length);
    ~TCPPacket();

    virtual unsigned char* get_data();
    virtual int get_data_length_bytes();

    virtual void set_data(unsigned char* data, int length);
    
    u_int32_t get_tcp_sequence_number();
    void set_tcp_sequence_number(u_int32_t seq_num);
    u_int32_t get_tcp_ack_number();
    void set_tcp_ack_number(u_int32_t ack_num);
    int get_tcp_header_length_bytes();
    u_int16_t get_tcp_header_length_words();
    void set_tcp_header_length_words(u_int16_t length);
    bool is_tcp_urg();
    void set_tcp_urg(bool urg);
    bool is_tcp_ack();
    void set_tcp_ack(bool ack);
    bool is_tcp_psh();
    void set_tcp_psh(bool psh);
    bool is_tcp_rst();
    void set_tcp_rst(bool rst);
    bool is_tcp_syn();
    void set_tcp_syn(bool syn);
    bool is_tcp_fin();
    void set_tcp_fin(bool fin);
    u_int16_t get_tcp_receive_window_size();
    void set_tcp_receive_window_size(u_int16_t window);
    u_int16_t get_tcp_checksum();
    void set_tcp_checksum(u_int16_t checksum);
    u_int16_t get_tcp_urgent_pointer();
    void set_tcp_urgent_pointer(u_int16_t urg_ptr);

    void init();

    bool is_naked_ack();

    virtual int max_data_length();

private:
    struct tcphdr* tcp_;

};

#endif	/* _TCPPACKET_H */

