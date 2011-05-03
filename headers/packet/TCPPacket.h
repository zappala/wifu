/* 
 * File:   TCPPacket.h
 * Author: rbuck
 *
 * Created on January 7, 2011, 2:42 PM
 */

#ifndef _TCPPACKET_H
#define	_TCPPACKET_H

#include "TCPHeaderOptionCollection.h"
#include "TCPHeaderOption.h"
#include "visitors/GetTCPHeaderOptionsLengthVisitor.h"
#include "exceptions/IllegalStateException.h"
#include "visitors/GetTCPHeaderOptionsDataVisitor.h"
#include "packet/WiFuPacket.h"
#include <netinet/tcp.h>

class TCPPacket : public WiFuPacket {
public:

    TCPPacket();
    //    TCPPacket(IPPacket&);
    //    TCPPacket(unsigned char* buffer, int length);
    ~TCPPacket();

    virtual unsigned char* get_data();
    virtual int get_data_length_bytes();

    virtual void set_data(unsigned char* data, int length);

    virtual void pack();

    u_int32_t get_tcp_sequence_number() const;
    void set_tcp_sequence_number(u_int32_t seq_num);
    u_int32_t get_tcp_ack_number() const;
    void set_tcp_ack_number(u_int32_t ack_num);
    int get_tcp_header_length_bytes() const;
    u_int16_t get_tcp_data_offset() const;
    void set_tcp_data_offset(u_int16_t length);
    bool is_tcp_urg() const;
    void set_tcp_urg(bool urg);
    bool is_tcp_ack() const;
    void set_tcp_ack(bool ack);
    bool is_tcp_psh() const;
    void set_tcp_psh(bool psh);
    bool is_tcp_rst() const;
    void set_tcp_rst(bool rst);
    bool is_tcp_syn() const;
    void set_tcp_syn(bool syn);
    bool is_tcp_fin() const;
    void set_tcp_fin(bool fin);
    u_int16_t get_tcp_receive_window_size() const;
    void set_tcp_receive_window_size(u_int16_t window);
    u_int16_t get_tcp_checksum() const;
    void set_tcp_checksum(u_int16_t checksum);
    u_int16_t get_tcp_urgent_pointer() const;
    void set_tcp_urgent_pointer(u_int16_t urg_ptr);

    void init();

    bool is_naked_ack();

    virtual int max_data_length();

    virtual string to_s() const;
    virtual string to_s_format() const;

    virtual bool operator ==(const IPPacket& other) const;
    virtual bool operator !=(const IPPacket& other) const;

    void insert_tcp_header_option(TCPHeaderOption* option);
    TCPHeaderOption* remove_tcp_header_option(u_int8_t kind);
    TCPHeaderOption* get_option(u_int8_t kind);

private:

    unsigned char* get_options_pointer();

    struct tcphdr* tcp_;

    TCPHeaderOptionCollection options_;
    bool data_set_;
};

#endif	/* _TCPPACKET_H */

