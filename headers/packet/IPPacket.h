/* 
 * File:   IPPacket.h
 * Author: rbuck
 *
 * Created on January 26, 2011, 2:07 PM
 */

#ifndef _IPPACKET_H
#define	_IPPACKET_H

#include <arpa/inet.h>
#include <netinet/ip.h>
#include "defines.h"
#include "GarbageCollector.h"
#include <sstream>
#include <string.h>
#include <iostream>

using namespace std;

class IPPacket : public gc {
public:

    IPPacket();
//    IPPacket(IPPacket&);
    IPPacket(unsigned char* buffer, int length);

    ~IPPacket();

    unsigned char* get_payload();

    virtual unsigned char* get_data();

    virtual int get_data_length_bytes();

    virtual void set_data(unsigned char* data, int length);

    virtual void pack();

    unsigned char* get_next_header();

    /**
     * Gets the IP version of this packet
     */
    u_int8_t get_ip_version() const;

    /**
     * Sets the IP version of this packet
     */
    void set_ip_version(u_int8_t version);

    /**
     * Gets the IP header length of this packet, in bytes
     */
    u_int8_t get_ip_header_length_bytes() const;

    /**
     * Gets the IP header length of this packet, in 32 bit words
     */
    u_int8_t get_ip_header_length_words() const;

    /**
     * Sets the length of the IP header of this packet, in 32 bit words
     */
    void set_ip_header_length_words(u_int8_t ihl);

    /**
     * Gets the IP TOS of this packet
     */
    u_int8_t get_ip_tos() const;

    /**
     * Sets the IP TOS of this packet
     */
    void set_ip_tos(u_int8_t tos);

    /**
     * Gets the IP datagram length of this packet
     */
    u_int16_t get_ip_tot_length() const;

    /**
     * Sets the IP datagram length of this packet
     */
    void set_ip_tot_length(u_int16_t length);

    /**
     * Gets the IP identifier of this packet
     */
    u_int16_t get_ip_identifier() const;

    /**
     * Sets the IP identifier of this packet
     */
    void set_ip_identifier(u_int16_t identifier);

    /**
     * Gets the IP fragmentation offset of this packet
     */
    u_int16_t get_ip_fragmentation_offset() const;

    /**
     * Sets the IP fragmentation offset of this packet
     */
    void set_ip_fragmentation_offset(u_int16_t frag_off);

    /**
     * Gets the IP TTL of this packet
     */
    u_int8_t get_ip_ttl() const;

    /**
     * Sets the IP TTL of this packet
     */
    void set_ip_ttl(u_int8_t ttl);

    /**
     * Gets the IP protocol of this packet
     */
    u_int8_t get_ip_protocol() const;

    /**
     * Sets the IP protocol of this packet
     */
    void set_ip_protocol(u_int8_t protocol);

    /**
     * Gets the IP checksum of this packet
     */
    u_int16_t get_ip_checksum() const;

    /**
     * Sets the IP checksum of this packet
     */
    void set_ip_checksum(u_int16_t checksum);

    /**
     * Gets the source IP address of this packet
     */
    u_int32_t get_ip_source_address() const;

    /**
     * Sets the source IP address of this packet
     */
    void set_ip_source_address(u_int32_t saddr);

    /**
     * Gets a string representation of the source IP address of this packet
     */
    string get_ip_source_address_s() const;

    /**
     * Sets a string representation of the source IP address of this packet
     */
    void set_ip_source_address_s(string saddr);

    /**
     * Gets the destination IP address of this packet
     */
    u_int32_t get_ip_destination_address() const;

    /**
     * Sets the destination IP address of this packet
     */
    void set_ip_destination_address(u_int32_t daddr);

    /**
     * Gets a string representation of the destination IP address of this packet
     */
    string get_ip_destination_address_s() const;

    /**
     * Sets a string representation of the destination IP address of this packet
     */
    void set_ip_destination_address_s(string daddr);

    void init();

    bool length_is_set() const;

    virtual string to_s() const;

    virtual string to_s_format() const;

    /**
     * @return MTU less the packet header(s)
     */
    virtual int max_data_length() const;

    virtual bool operator ==(const IPPacket& other) const;

    virtual bool operator !=(const IPPacket& other) const;

private:
    unsigned char payload_[MTU];
    struct iphdr* ip_;
    bool length_set_;
};

ostream& operator <<(ostream& os, const IPPacket& packet);

#endif	/* _IPPACKET_H */
