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

using namespace std;

/**
 * Represents a TCPPacket with a header and payload.
 * All header variables 16 bits and bigger are stored in network byte order.
 * Conversion occurs to network byte order when each variable is set and from network byte order when a variable is accessed.
 * This means that programmers need not worry about conversion themselves.
 *
 * There are a lot of virtual methods.  Most of which MUST be overridden by children classes.
 * Some of these which must be overridden MUST call the parent version of the method in their implementation.
 *
 * This class inherits from WiFuPacket and uses the getters and setters for ports, but assumes responsibility for the ports in the sense that it is part of the tcp header.
 *
 * @see IPPacket
 * @see WiFuPacket
 */
class TCPPacket : public WiFuPacket {
public:

    /**
     * Constructor.
     * Sets the TCP header to immediately follow the IP header.
     * Sets the TCP data offset to 5 (32-bit words).
     * Sets the IP total length to IP plus TCP header lengths.
     */
    TCPPacket();

    /**
     * Destructor.
     */
    ~TCPPacket();

    /**
     * Returns the first data byte after the IP and TCP headers.
     * Child classes MUST override this method to ensure that they account for any underlying protocol headers.
     *
     * @return The first data byte after the IP and TCP headers.
     * @see IPPacket::get_data()
     * @see WiFuPacket::get_data()
     */
    virtual unsigned char* get_data();

    /**
     * Returns the length of the data AFTER the IP and TCP headers.
     * Child classes MUST override this method to ensure that they account for any underlying protocol headers.
     *
     * @return The length of the data AFTER the IP and TCP headers.
     * @see IPPacket::get_data_length_bytes()
     * @see WiFuPacket::get_data_length_bytes()
     */
    virtual int get_data_length_bytes();

    /**
     * First, copies length bytes from data into the buffer after the IP and TCP headers (including any options) of this packet and
     * second, sets the IPPacket total length field.
     *
     * Child classes MUST override this method to ensure that they account for any underlying protocol headers.
     * Namely, data must be set after any headers AND they MUST call IPPacket::set_ip_tot_length().
     *
     * @param data Pointer to a buffer containing data.
     * @param length Length of data in bytes.
     *
     * @see IPPacket::set_data()
     * @see IPPacket::set_ip_tot_length()
     * @see WiFuPacket::set_data()
     * @see WiFuPacket::set_ip_tot_length()
     *
     * @throw IllegalStateException if this function is called more than once.
     * This is because we do not want to add any more TCP options after the data has been set (to avoid shifting the data to add room for a new option).
     * The big problem with shifting data is that we may not have room to shift it.
     */
    virtual void set_data(unsigned char* data, int length);

    /**
     * Inserts all TCP header options and appends any necessary padding to the options.
     * 
     * @see IPPacket::pack()
     * @see WiFuPacket::pack()
     */
    virtual void pack();

    /**
     * @return The TCP sequence number.
     */
    u_int32_t get_tcp_sequence_number() const;

    /**
     * Sets the TCP sequence number.
     * @param seq_num The host byte ordered sequence number to set.
     */
    void set_tcp_sequence_number(u_int32_t seq_num);

    /**
     * @return The TCP acknowledgement number.
     */
    u_int32_t get_tcp_ack_number() const;

    /**
     * Sets the TCP acknowledgement number.
     * @param ack_num The host byte ordered acknowledgement number to set.
     */
    void set_tcp_ack_number(u_int32_t ack_num);

    /**
     * @return The TCP header length in bytes.
     */
    int get_tcp_header_length_bytes() const;

    /**
     * @return The TCP header length in 32-bit words.
     */
    u_int16_t get_tcp_data_offset() const;

    /**
     * Sets the TCP header length in 32-bit words.
     * @param length The host byte ordered header length in 32-bit words.
     */
    void set_tcp_data_offset(u_int16_t length);

    /**
     * @return True if the URG bit is set, false otherwise.
     */
    bool is_tcp_urg() const;

    /**
     * Sets the URG bit to urg.
     * @param urg Value to set the URG bit to.
     */
    void set_tcp_urg(bool urg);

    /**
     * @return True if the ACK bit is set, false otherwise.
     */
    bool is_tcp_ack() const;

    /**
     * Sets the ACK bit to urg.
     * @param urg Value to set the ACK bit to.
     */
    void set_tcp_ack(bool ack);

    /**
     * @return True if the PSH bit is set, false otherwise.
     */
    bool is_tcp_psh() const;

    /**
     * Sets the PSH bit to urg.
     * @param urg Value to set the PSH bit to.
     */
    void set_tcp_psh(bool psh);

    /**
     * @return True if the RST bit is set, false otherwise.
     */
    bool is_tcp_rst() const;

    /**
     * Sets the RST bit to urg.
     * @param urg Value to set the RST bit to.
     */
    void set_tcp_rst(bool rst);

    /**
     * @return True if the SYN bit is set, false otherwise.
     */
    bool is_tcp_syn() const;

    /**
     * Sets the SYN bit to urg.
     * @param urg Value to set the SYN bit to.
     */
    void set_tcp_syn(bool syn);

    /**
     * @return True if the FIN bit is set, false otherwise.
     */
    bool is_tcp_fin() const;

    /**
     * Sets the FIN bit to urg.
     * @param urg Value to set the FIN bit to.
     */
    void set_tcp_fin(bool fin);

    /**
     * @return The TCP receive window size.
     */
    u_int16_t get_tcp_receive_window_size() const;

    /**
     * Sets the TCP receive window size.
     * @param window The host byte ordered receive window size to set.
     */
    void set_tcp_receive_window_size(u_int16_t window);

    /**
     * @return The TCP checksum.
     */
    u_int16_t get_tcp_checksum() const;

    /**
     * Sets the TCP checksum.
     * @param checksum The checksum to set.
     */
    void set_tcp_checksum(u_int16_t checksum);

    /**
     * @return The TCP urgent pointer.
     */
    u_int16_t get_tcp_urgent_pointer() const;

    /**
     * Sets the TCP urgent pointer.
     * @param urg_ptr The host byte ordered urgent pointer to set.
     */
    void set_tcp_urgent_pointer(u_int16_t urg_ptr);

    /**
     * Calculates and sets the TCP checksum.
     * The TCP checksum is computed over the pseudo header, TCP header, and the TCP data.
     * @see RFC 793.
     */
    void calculate_and_set_tcp_checksum();

    /**
     * Validates that the current TCP checksum is correct.
     * This method has no side-effects.
     *
     * @return True of the current TCP checksum is corrrect, false otherwise.
     */
    bool is_valid_tcp_checksum();

    /**
     * A naked ack is deemed to be a TCP packet with no data AND only the ACK bit is set.
     * @return True if this TCPPacket is a naked ack, false otherwise.
     */
    bool is_naked_ack();

    /**
     * Returns the maximum number of bytes that this packet can store after the IP and TCP headers.
     * For TCPPacket this is MTU minus the IP and TCP header lengths (including any TCP header options).
     *
     * Child classes MUST override this method to ensure that they account for any underlying protocol headers.
     *
     * @return The maximum number of bytes that this packet can store after the IP and TCP headers.
     *
     * @see IPPacket::max_data_length()
     * @see WiFuPacket::max_data_length()
     */
    virtual int max_data_length();

    /**
     * @return A string representation of the fields in the IP and TCP headers.
     *
     * Child classes MUST override this method to ensure that they account for any underlying protocol headers.
     * A child class MUST call TCPPacket::to_s() first and append the next protocol's header data on the next line.
     *
     * @see IPPacket::to_s()
     * @see WiFuPacket::to_s()
     * @see TCPPacket::to_s_format()
     */
    virtual gcstring to_s() const;

    /**
     * @return A string representing the names of the values produced by TCPPacket::to_s().
     *
     * A child class MUST call TCPPacket::to_s() first and append the next protocol's header names (which must contain ports) on the next line.
     *
     * @see IPPacket::to_s_format()
     * @see WiFuPacket::to_s_format()
     * @see TCPPacket::to_s()
     */
    virtual gcstring to_s_format() const;

    /**
     * Compares this TCPPacket with other and returns whether they are equal or not.
     * First, it checks the IPPacket and WiFuPacket headers for equallity; second, it checks to ensure that it is a TCPPacket, and finally it checks the TCPPacket header.
     *
     * Child classes MUST override this method first calling the parent (this) function to ensure that they account for any underlying protocol equallity AND IP and WiFu equallity.
     *
     * @param other IPPacket (which is actually a TCPPacket) to compare with this WiFuPacket
     * @return True if every field in the IP, WiFu, and TCP headers are equal in this WiFuPacket and other, false otherwise.
     * Note, we do not compare the TCP checksum value or any TCP options.
     *
     * @see IPPacket::operator==
     * @see WiFuPacket::operator==
     */
    virtual bool operator ==(const IPPacket& other) const;

    /**
     * Compares this TCPPacket with other and returns whether they are not equal.
     *
     * Child classes MUST override this method first calling the parent (this) function to ensure that they account for any underlying protocol inequallity AND IP inequallity.
     *
     * @param other IPPacket to compare with this TCPPPacket
     * @return the opposite of TCPPacket::operator==
     */
    virtual bool operator !=(const IPPacket& other) const;

    /**
     * Inserts option into the collection of options.
     * Note that this does not actually insert the option into the header.
     * Options are inserted upon calling TCPPacket::pack()
     * 
     * @param option The TCPHeaderOption to insert.
     */
    virtual void insert_tcp_header_option(TCPHeaderOption* option);

    /**
     * Removes kind from the TCPHeader options.
     * @param kind The option kind to remove.
     * @return The option removed, NULL if it was not in the collection.
     * @see TCPPacket::insert_tcp_header_option()
     */
    virtual TCPHeaderOption* remove_tcp_header_option(u_int8_t kind);

    /**
     * Returns the option denoted by kind if it exists.
     * This function parses the TCP header options if this packet was not created by this application (e.g. it was received over the network).
     * 
     * @param kind The integer found in netinet/tcp.h which denotes the option kind.  All options begin with "TCPOPT_".
     * @return The TCPHeaderOption denoted by kind if it exists, NULL otherwise.
     */
    TCPHeaderOption* get_option(u_int8_t kind);


private:

    /**
     * Sets the TCP header to immediately follow the IP header.
     * Sets the TCP data offset to 5 (32-bit words).
     * Sets the IP total length to IP plus TCP header lengths.
     *
     * @see TCPPacket::TCPPacket()
     */
    void init();

    /**
     * @return A pointer immediately following the main TCP header (where the TCP options begin).
     */
    unsigned char* get_options_pointer();

    /**
     * Pointer to the TCP header.
     */
    struct tcphdr* tcp_;

    /**
     * Collection of TCPHeaderOption objects to insert as TCP options.
     */
    TCPHeaderOptionCollection options_;

    /**
     * Used to tell whether we have called TCPPacket::set_data() or not.
     * We can only call this method once.
     */
    bool data_set_;
};

#endif	/* _TCPPACKET_H */

