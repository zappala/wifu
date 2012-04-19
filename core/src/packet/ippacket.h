#ifndef _ippacket_h
#define	_ippacket_h

// includes
#include <arpa/inet.h>
#include <netinet/ip.h>

// C++ includes
#include <sstream>

// local includes
#include "packet.h"
#include "wifu_protocol.h"

class IPPacket : public Packet {

public:

    // Constructor that sets the following class variables:
    //
    // qh: netfilter queue handle
    // id: netfilter id
    // payload_length: size in bytes of payload
    // payload: pointer to the buffer containing an ip packet
    // filter: enum type of which queue this Packet came from
    // preserve_payload: if true, make a copy of the payload
    IPPacket(struct nfq_q_handle* qh, int id, int payload_length,
             unsigned char* payload, Filter filter,
             bool preserve_payload = true);

    // Constructor that copies the contents of an IP packet at initialization
    // 
    // p: IP packet to copy into this packet
    // preserve_payload: if true, make a copy of the payload
    IPPacket(Packet& p, bool preserve_payload = true);

    virtual ~IPPacket();

    // Gets a pointer to the IP header associated with this packet
    struct iphdr* ip();

    // ================================================================
    // The following methods are getters and setters for the IP header
    // ================================================================

    // Gets the IP version of this packet
    u_int8_t get_ip_version();

    // Sets the IP version of this packet
    void set_ip_version(u_int8_t version);

    // Gets the IP header length of this packet, in bytes
    u_int8_t get_ip_length_bytes();

    // Gets the IP header length of this packet, in 32 bit words
    u_int8_t get_ip_length();

    // Sets the length of the IP header of this packet, in 32 bit words
    void set_ip_length(u_int8_t ihl);

    // Gets the IP TOS of this packet
    u_int8_t get_ip_tos();

    // Sets the IP TOS of this packet
    void set_ip_tos(u_int8_t tos);

    // Gets the IP datagram length of this packet
    u_int16_t get_ip_datagram_length();

    // Sets the IP datagram length of this packet
    void set_ip_datagram_length(u_int16_t length);

    // Gets the IP identifier of this packet
    u_int16_t get_ip_identifier();

    // Sets the IP identifier of this packet
    void set_ip_identifier(u_int16_t identifier);

    // Gets the IP fragmentation offset of this packet
    u_int16_t get_ip_fragmentation_offset();

    // Sets the IP fragmentation offset of this packet
    void set_ip_fragmentation_offset(u_int16_t frag_off);

    // Gets the IP TTL of this packet
    u_int8_t get_ip_ttl();

    // Sets the IP TTL of this packet
    void set_ip_ttl(u_int8_t ttl);

    // Gets the IP protocol of this packet
    u_int8_t get_ip_protocol();

    // Sets the IP protocol of this packet
    void set_ip_protocol(u_int8_t protocol);

    // Gets the IP checksum of this packet
    u_int16_t get_ip_checksum();

    // Sets the IP checksum of this packet
    void set_ip_checksum(u_int16_t checksum);

    // Gets the source IP address of this packet
    u_int32_t get_ip_source_address();

    // Sets the source IP address of this packet
    void set_ip_source_address(u_int32_t saddr);

    // Gets a string representation of the source IP address of this packet
    string get_ip_source_address_s();

    // Sets a string representation of the source IP address of this packet
    void set_ip_source_address_s(string saddr);

    // Gets the destination IP address of this packet
    u_int32_t get_ip_destination_address();

    // Sets the destination IP address of this packet
    void set_ip_destination_address(u_int32_t daddr);

    // Gets a string representation of the destination IP address of this packet
    string get_ip_destination_address_s();

    // Sets a string representation of the destination IP address of this packet
    void set_ip_destination_address_s(string daddr);

    // ================================================================
    // The following methods are for convenience in handling IP packets
    // ================================================================

    // Gets a string representation of this packet.  See to_s_format
    // for a definition of what is actually returned
    virtual string to_s();

    // Format of the string representation of this packet.
    // For the IP header, this is:
    //
    // # ip source destination protocol datagram_length ihl ttl
    virtual string to_s_format();

    // Recalculates the IP checksum of this packet
    void recalculate_ip_checksum();

    // Makes room for a shim header in a packet
    //
    // length: the length of the shim, not including the WiFu header
    virtual void make_room_for_shim(int length);

    // Resets header pointer after shim inserted or removed
    virtual void cleanup_shim();

    // gets the next header of this packet, skipping over any WiFu
    // or shim headers
    unsigned char* get_next_header();

    // insert a shim header into this packet, after the IP header and
    // before any following header
    //
    // shim: pointer to a shim header
    // shim_type: enumerated type of the shim header
    // shim_length: length of the shim header in bytes
    void insert_shim(unsigned char* shim, u_int8_t shim_type,
                     u_int8_t shim_length);

    // get a pointer to the WiFu header
    struct wifuhdr* get_wifu();

    // get a pointer to the shim header
    unsigned char* get_shim();

    // remove the shim header from this packet
    void remove_shim();

    // calculate the Internet checksum
    //
    // ptr: a sequence of unsigned 16-bit integers
    // len: length of the sequence
    static u_int16_t checksum(u_int16_t* ptr, u_int16_t len);

    // calculate the UDP or TCP checksum
    u_int16_t udptcp_checksum(void* header);

private:

    // pointer inside packet's payload to the IP header
    struct iphdr* ip_;
};

// structure for the IP pseudo header, needed for the UDP and TCP checksum
struct ip_pseudo_header {
    u_int32_t saddr;
    u_int32_t daddr;
    u_int8_t zero;
    u_int8_t protocol;
    u_int16_t tot_len;
};

#endif	// _ippacket_h
