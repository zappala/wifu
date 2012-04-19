#ifndef _udppacket_h
#define	_udppacket_h

// includes
#include <netinet/udp.h>

// C++ includes
#include <iostream>

// local includes
#include "ippacket.h"
#include "wifu_protocol.h"

class UDPPacket : public IPPacket {
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
    UDPPacket(struct nfq_q_handle* qh, int id, int payload_length,
              unsigned char* payload, Filter filter,
              bool preserve_payload = true);

    // Constructor that copies the contents of a UDP packet at initialization
    // 
    // p: UDP packet to copy into this packet
    // preserve_payload: if true, make a copy of the payload
    UDPPacket(Packet& p, bool preserve_payload = true);

    virtual ~UDPPacket();

    // Gets a pointer to the UDP header associated with this packet
    struct udphdr* udp();

    // ================================================================
    // The following methods are getters and setters for the UDP header
    // ================================================================

    // Gets the UDP source port of this packet
    u_int16_t get_udp_source_port();

    // Sets the UDP source port of this packet
    void set_udp_source_port(u_int16_t port);

    // Gets the UDP destination port of this packet
    u_int16_t get_udp_destination_port();

    // Sets the UDP destination port of this packet
    void set_udp_destination_port(u_int16_t port);

    // Gets the UDP length (header + data)
    u_int16_t get_udp_length();

    // Sets the UDP length (header + data)
    void set_udp_length(u_int16_t length);

    // Gets the UDP checksum in this packet
    u_int16_t get_udp_checksum();

    // Sets the TCP checksum of this packet
    void set_udp_checksum(u_int16_t checksum);

    // ================================================================
    // The following methods are for convenience in handling UDP packets
    // ================================================================

    // Gets a string representation of this packet.  See to_s_format
    // for a definition of what is actually returned
    virtual string to_s();

    // Format of the string representation of this packet.
    // For the UDP header, this is:
    //
    // # udp sport dport length
    virtual string to_s_format();

    // Recalculates the UDP checksum in this packet
    void recalculate_udp_checksum();

    // Makes room for a shim header in a UDP packet
    //
    // length: the length of the shim, not including the WiFu header
    virtual void make_room_for_shim(u_int8_t length);

    // Resets UDP header pointer after shim inserted or removed
    virtual void cleanup_shim();

private:

    // pointer to the UDP header inside the packet's payload
    struct udphdr* udp_;
};

#endif	// _udppacket_h
