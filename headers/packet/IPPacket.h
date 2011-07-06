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
#include <sstream>
#include <string.h>
#include <iostream>

#include "defines.h"
#include "GarbageCollector.h"

using namespace std;

/**
 * Pseudo header used in computing TCP and UDP checksums.
 * For more information see: http://www.tcpipguide.com/free/t_TCPChecksumCalculationandtheTCPPseudoHeader-2.htm which has a nice tutorial.
 * Note that computing the TCP and UDP checksums uses this pseudo header.
 */
struct ip_pseudo_header {
    /**
     * Source address.
     */
    u_int32_t saddr;

    /**
     * Destination address.
     */
    u_int32_t daddr;

    /**
     * Padding (zeros).
     */
    u_int8_t zero;

    /**
     * The protocol field from the IP header.
     */
    u_int8_t protocol;

    /**
     * Length of the transport layer header and the data (Total length minus the IP header length).
     */
    u_int16_t tot_len;
};

/**
 * Represents an IPPacket with a header and payload.
 * It is intended that underlying protocols (such as TCP) will inherit from this class to create their own packet type.
 * All header variables 16 bits and bigger are stored in network byte order.
 * Conversion occurs to network byte order when each variable is set and from network byte order when a variable is accessed.
 * This means that programmers need not worry about conversion themselves.
 *
 * There are a lot of virtual methods.  Most of which MUST be overridden by children classes.
 * Some of these which must be overridden MUST call the parent version of the method in their implementation.
 *
 * @see WiFuPacket
 * @see TCPPacket
 */
class IPPacket : public gc {
public:

    /**
     * Default Constructor.
     * Sets IP version to 4, IP header length to 5 (32 bit words), IP total length to 20, and the TTL to MAX_TTL (65).
     *
     * @see IPPacket::init()
     */
    IPPacket();

    /**
     * Destructor.
     */
    ~IPPacket();

    /**
     * @return The first byte of this packet's payload (this is also the first byte of the ip header).
     */
    unsigned char* get_payload();

    /**
     * Returns the first data byte after the IP header.
     * Child classes MUST override this method to ensure that they account for any underlying protocol headers.
     * 
     * @return The first data byte after the IP header.
     */
    virtual unsigned char* get_data();

    /**
     * Returns the length of the data AFTER the IP header.
     * Child classes MUST override this method to ensure that they account for any underlying protocol headers.
     *
     * @return The length of the data AFTER the IP header.
     */
    virtual int get_data_length_bytes();

    /**
     * First, copies length bytes from data into the buffer after the IP header of this packet and
     * second, sets the IPPacket total length field.
     *
     * Child classes MUST override this method to ensure that they account for any underlying protocol headers.
     * Namely, data must be set after any headers AND they must call IPPacket::set_ip_tot_length().
     *
     * @param data Pointer to a buffer containing data.
     * @param length Length of data in bytes.
     *
     * @see IPPacket::set_ip_tot_length()
     */
    virtual void set_data(unsigned char* data, int length);

    /**
     * Finalizer method to call before a packet is sent.
     * It is intended to use for things like header options where it is unkown how many or how long each one will be for each packet.
     * It would be expensive to shift the data each time an option is inserted or removed.
     * A programmer could store the options in a data structure and insert them all at once via this function.
     * 
     * The default implementation is empty.
     *
     * Child classes SHOULD override this method.
     *
     * @see TCPPacket::pack()
     */
    virtual void pack();

    /**
     * @return A pointer to the byte following the ip header.
     */
    unsigned char* get_next_header();

    /**
     * @return The IP version of this packet.
     */
    u_int8_t get_ip_version() const;

    /**
     * Sets the IP version of this packet.
     *
     * @param version The version to set.
     */
    void set_ip_version(u_int8_t version);

    /**
     * @return The IP header length of this packet, in bytes.
     */
    u_int8_t get_ip_header_length_bytes() const;

    /**
     * @return The IP header length of this packet, in 32-bit words.
     */
    u_int8_t get_ip_header_length_words() const;

    /**
     * Sets the length of the IP header of this packet, in 32-bit words.
     *
     * @param ihl The Internet header length to set.
     */
    void set_ip_header_length_words(u_int8_t ihl);

    /**
     * @return The IP type of service of this packet.
     */
    u_int8_t get_ip_tos() const;

    /**
     * Sets the IP type of service of this packet.
     *
     * @tos The type of service to set.
     */
    void set_ip_tos(u_int8_t tos);

    /**
     * @return The IP datagram length of this packet in bytes.  This includes all headers and all data.
     */
    u_int16_t get_ip_tot_length() const;

    /**
     * Sets the IP datagram length of this packet in bytes.  This includes all headers and all data.
     * This function MUST be called before a packet can be sent.
     *
     * @param length The IP datagram length in bytes to set.
     */
    void set_ip_tot_length(u_int16_t length);

    /**
     * @return The IP identifier of this packet.
     */
    u_int16_t get_ip_identifier() const;

    /**
     * Sets the IP identifier of this packet.
     *
     * @param identifier The id of this packet to set.
     */
    void set_ip_identifier(u_int16_t identifier);

    /**
     * @return The IP fragmentation offset of this packet.
     */
    u_int16_t get_ip_fragmentation_offset() const;

    /**
     * Sets the IP fragmentation offset of this packet.
     *
     * @param frag_off The IP fragmentation offset to set.
     */
    void set_ip_fragmentation_offset(u_int16_t frag_off);

    /**
     * @return The IP time to live (TTL) of this packet.
     */
    u_int8_t get_ip_ttl() const;

    /**
     * Sets the IP time to live (TTL) of this packet.
     *
     * @param ttl The time to live to set.
     */
    void set_ip_ttl(u_int8_t ttl);

    /**
     * @return The underlying protocol of this packet (TCP, UDP, etc.).
     */
    u_int8_t get_ip_protocol() const;

    /**
     * Sets the underlying protocol of this packet (TCP, UDP, etc.).
     *
     * @param protocol The underlying protocol to set.
     */
    void set_ip_protocol(u_int8_t protocol);

    /**
     * @return The IP checksum of the this packet.  (This is never stored in network byte order.)
     */
    u_int16_t get_ip_checksum() const;

    /**
     * Sets the IP checksum of this packet.  (This is never stored in network byte order.)
     *
     * @param checkusm The IP checksum of this packet.
     */
    void set_ip_checksum(u_int16_t checksum);

    /**
     * @return The source IP address of this packet.
     */
    u_int32_t get_ip_source_address() const;

    /**
     * Sets the source IP address of this packet.
     *
     * @param sadder The source IP address of this packet.
     */
    void set_ip_source_address(u_int32_t saddr);

    /**
     * @return A human-readable (e.g. 127.0.0.1) string representation of the source IP address of this packet.
     */
    string get_ip_source_address_s() const;

    /**
     * Sets a string representation of the source IP address of this packet.
     *
     * @param saddr A human-readable (e.g. 127.0.0.1) string representation of the source IP address of this packet.
     */
    void set_ip_source_address_s(string& saddr);

    /**
     * @return The destination IP address of this packet.
     */
    u_int32_t get_ip_destination_address() const;

    /**
     * Sets the destination IP address of this packet.
     *
     * @param The destination IP address of this packet.
     */
    void set_ip_destination_address(u_int32_t daddr);

    /**
     * @return A human-readable (e.g. 127.0.0.1) string representation of the destination IP address of this packet.
     */
    string get_ip_destination_address_s() const;

    /**
     * Sets a string representation of the destination IP address of this packet.
     *
     * @param daddr A human-readable (e.g. 127.0.0.1) string representation of the destination IP address of this packet.
     */
    void set_ip_destination_address_s(string& daddr);

    /**
     * Computes the 16-bit one's complement of the one's complement sum of all 16-bit words in buffer pointed to by ptr.
     *
     * @param ptr Pointer to data to compute the checksum over.
     * @param len Length of data to compute the checksum over.
     * 
     * @return The 16-bit one's complement of the one's complement sum of all 16-bit words in buffer pointed to by ptr.
     */
    static u_int16_t checksum(u_int16_t* ptr, u_int16_t len);

    /**
     * Calculates and sets the IP checksum.
     * The IP checksum is only computed over the IP header.
     */
    void calculate_and_set_ip_checksum();

    /**
     * Validates that the current IP checksum is correct.
     * This method has no side-effects.
     *
     * @return True of the current IP checksum is corrrect, false otherwise.
     */
    bool is_valid_ip_checksum();

    /**
     * Computes the checksum after the ip header for protocols that use the pseudo header.
     * @return The next checksum for protocols that use the pseudo header.
     */
    u_int16_t compute_next_checksum();

    /**
     * @return True iff IPPacket::set_ip_tot_length() has been called, false otherwise.
     * Calling IPPacket::set_data() calls IPPacket::set_ip_tot_length().
     *
     * @see IPPacket::set_ip_tot_length()
     * @see IPPacket::set_data()
     */
    bool length_is_set() const;

    /**
     * @return A string representation of the fields in the IP header.
     *
     * Child classes MUST override this method to ensure that they account for any underlying protocol headers.
     * A child class MUST call IPPacket::to_s() first and append the next protocol's header data on the next line.
     *
     * @see IPPacket::to_s_format()
     */
    virtual string to_s() const;

    /**
     * @return A string representing the names of the values produced by IPPacket::to_s().
     *
     * A child class MUST call IPPacket::to_s_format() first and append the next protocol's header names on the next line.
     *
     * @see IPPacket::to_s()
     */
    virtual string to_s_format() const;

    /**
     * Returns the maximum number of bytes that this packet can store after the IP header.
     * For an IP packet this is MTU minus the IP header length.
     * 
     * Child classes MUST override this method to ensure that they account for any underlying protocol headers.
     *
     * @return The maximum number of bytes that this packet can store after the IP header.
     */
    virtual int max_data_length() const;

    /**
     * Compares this IPPacket with other and returns whether they are equal or not.
     *
     * Child classes MUST override this method first calling the parent (this) function to ensure that they account for any underlying protocol equallity AND ip equallity.
     *
     * @param other IPPacket to compare with this IPPacket
     * @return True if every field in the headers are equal in this IPPacket and other, false otherwise.
     * Note, we do not compare the checksum value and don't need to as the checksum is computed over the exact same fields we are comparing.
     */
    virtual bool operator==(const IPPacket& other) const;

    /**
     * Compares this IPPacket with other and returns whether they are not equal.
     *
     * Child classes MUST override this method first calling the parent (this) function to ensure that they account for any underlying protocol equallity AND ip inequallity.
     *
     * @param other IPPacket to compare with this IPPacket
     * @return the opposite of IPPacket::operator==
     */
    virtual bool operator!=(const IPPacket& other) const;

private:


    /**
     * Initializes this IPPacket.
     * Sets IP version to 4, IP header length to 5 (32-bit words), IP total length to 20, and the TTL to MAX_TTL (65).
     *
     * @see IPPacket::IPPacket()
     */
    void init();

    /**
     * Payload of this packet.  This is where all headers and data are stored for all protocols.
     */
    unsigned char payload_[MTU];

    /**
     * Pointer to the IP header.
     */
    struct iphdr* ip_;

    /**
     * Used to tell whether the length has been set.
     * We need to know the length when we send packets over the network because we send IPPacket::get_ip_tot_length() bytes when we actually send the data.
     */
    bool length_set_;
};

/**
 * Stream operator for an IPPacket.
 * @param os ostream to write to.
 * @param packet An IPPacket to output.
 * @return packet's version of IPPacket::to_s_format(), followed by IPPacket::to_s().
 */
ostream & operator <<(ostream& os, const IPPacket& packet);

#endif	/* _IPPACKET_H */
