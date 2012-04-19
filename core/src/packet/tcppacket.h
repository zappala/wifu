#ifndef _tcppacket_h
#define	_tcppacket_h

// includes
#include <netinet/tcp.h>

// C++ includes
#include <iostream>

// local includes
#include "ippacket.h"
#include "wifu_protocol.h"

class TCPPacket : public IPPacket {

public:

    // Constructor that sets the following class variables:
    //
    // qh: netfilter queue handle
    // id: netfilter id
    // payload_length: size in bytes of payload
    // payload: pointer to the buffer containing an ip packet
    // filter: enum type of which queue this Packet came from
    // preserve_payload: indicates whether we want to malloc the payload
    //                   on the heap
    TCPPacket(struct nfq_q_handle* qh, int id, int payload_length,
              unsigned char* payload, Filter filter,
              bool preserve_payload = true);

    // Constructor that copies the contents of a TCP packet at initialization
    // 
    // p: TCP packet to copy into this packet
    // preserve_payload: if true, make a copy of the payload
    TCPPacket(Packet& p, bool preserve_payload = true);

    virtual ~TCPPacket();

    // Gets a pointer to the TCP header associated with this packet
    struct tcphdr* tcp();

    // ================================================================
    // The following methods are getters and setters for the TCP header
    // ================================================================

    // Gets the TCP source port of this packet
    u_int16_t get_tcp_source_port();

    // Sets the TCP source port of this packet
    void set_tcp_source_port(u_int16_t port);

    // Gets the TCP destination port of this packet
    u_int16_t get_tcp_destination_port();

    // Sets the TCP destination port of this packet
    void set_tcp_destination_port(u_int16_t port);

    // Gets the TCP sequcence number of this packet
    u_int32_t get_tcp_sequence_number();

    // Sets the TCP sequcence number of this packet
    void set_tcp_sequence_number(u_int32_t seq_num);

    // Gets the TCP acknowledgement number of this packet
    u_int32_t get_tcp_ack_number();

    // Sets the TCP acknowledgement number of this packet
    void set_tcp_ack_number(u_int32_t ack_num);

    // Gets the TCP header length of this packet, in bytes
    int get_tcp_length_bytes();

    // Gets the TCP header length of this packet, in 32-bit words
    u_int16_t get_tcp_length();

    // Sets the TCP header length of this packet, in 32-bit words
    void set_tcp_length(u_int16_t length);

    // Checks whether the TCP URG bit is set in this packet.
    bool is_tcp_urg();

    // Sets the TCP URG bit in this packet
    void set_tcp_urg(bool urg);

    // Checks whether the TCP ACK bit is set in this packet
    bool is_tcp_ack();

    // Sets the TCP ACK bit in this packet
    void set_tcp_ack(bool ack);

    // Checks whether the TCP PSH bit is set in this packet
    bool is_tcp_psh();

    // Sets the TCP PSH bit in this packet
    void set_tcp_psh(bool psh);

    // Checks whether the TCP RST bit is set in this packet
    bool is_tcp_rst();

    // Sets the TCP RST bit in this packet
    void set_tcp_rst(bool rst);

    // Checks whether the TCP SYN bit is set in this packet
    bool is_tcp_syn();

    // Sets the TCP SYN bit in this packet
    void set_tcp_syn(bool syn);

    // Checks whether the TCP FIN bit is set in this packet
    bool is_tcp_fin();

    // Sets the TCP FIN bit in this packet
    void set_tcp_fin(bool fin);

    // Gets the TCP receive window size in this packet
    u_int16_t get_tcp_receive_window_size();

    // Sets the TCP receive window size in this packet
    void set_tcp_receive_window_size(u_int16_t window);

    // Gets the TCP checksum in this packet
    u_int16_t get_tcp_checksum();

    // Sets the TCP checksum in this packet
    void set_tcp_checksum(u_int16_t checksum);
    
    // Gets the TCP urgent data pointer in this packet
    u_int16_t get_tcp_urgent_pointer();

    // Sets the TCP urgent data pointer in this packet
    void set_tcp_urgent_pointer(u_int16_t urg_ptr);

    // ================================================================
    // The following methods are for convenience in handling TCP packets
    // ================================================================

    // Gets a string representation of this packet.  See to_s_format
    // for a definition of what is actually returned
    virtual string to_s();

    // Format of the string representation of this packet.
    // For the TCP header, this is:
    //
    // # tcp sport dport seq_num ack_num header_length URG ACK PSH RST SYN FIN
    virtual string to_s_format();

    // Recalculates the TCP ckecksum in this packet
    void recalculate_tcp_checksum();

    // Makes room for a shim header in a TCP packet
    //
    // length: the length of the shim, not including the WiFu header
    void make_room_for_shim(int length);

    // Resets TCP header pointer after shim inserted or removed
    void cleanup_shim();

private:
   
    // pointer to the TCP header inside the packet's payload
    struct tcphdr* tcp_;

    // set the TCP MAXSEG option
    void set_tcp_maxseg_option(u_int16_t extra);

    // get the TCP Timestamp option
    void get_tcp_timestamp_option(struct tcp_timestamp_option* ts_option);
};

// structure for the TCP MAXSEG option
struct tcp_maxseg_option {
    u_int16_t mss;
};

//structure for the TCP TIMESTAMP option
struct tcp_timestamp_option {
    // timestamp value: current value of the timestamp clock of the
    // TCP sending the option
    u_int32_t TSval;
    // timestamp echo reply: echo of the most recent timestamp option
    // received by the TCP sending the option
    u_int32_t TSecr;
};

#endif	// _tcppacket_h
