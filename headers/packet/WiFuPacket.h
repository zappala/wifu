/* 
 * File:   WiFuPacket.h
 * Author: rbuck
 *
 * Created on January 26, 2011, 2:36 PM
 */

#ifndef _WIFUPACKET_H
#define	_WIFUPACKET_H

#include "IPPacket.h"
#include "AddressPort.h"
#include <sstream>

using namespace std;

/**
 * Provides structure for ports.
 */
struct wifu_packet_header {
    /**
     * Source port.
     */
    u_int16_t sport;

    /**
     * Destination port.
     */
    u_int16_t dport;
};

/**
 * Base class that enforces the useage of a source port followed by a destination port immediately following the IP header.
 * This ensures that all protocols using WiFu must use ports.
 * We need ports to multiplex/demulitplex packets coming and going over the network interface.
 *
 * @see IPPacket
 * @see TCPPacket
 */
class WiFuPacket : public IPPacket {
public:

    /**
     * Constructs a WiFuPacket.
     *
     * @see WiFuPacket::init()
     */
    WiFuPacket();

    /**
     * Destructor.
     */
    ~WiFuPacket();

    /**
     * Returns the first data byte after the IP and WiFu headers.
     * Child classes MUST override this method to ensure that they account for any underlying protocol headers.
     *
     * @return The first data byte after the IP and WiFu headers.
     * @see IPPacket::get_data()
     */
    virtual unsigned char* get_data();

    /**
     * Returns the length of the data AFTER the IP and WiFu headers.
     * Child classes MUST override this method to ensure that they account for any underlying protocol headers.
     *
     * @return The length of the data AFTER the IP and WiFu headers.
     * @see IPPacket::get_data_length_bytes()
     */
    virtual int get_data_length_bytes();

    /**
     * First, copies length bytes from data into the buffer after the IP and WiFu headers of this packet and
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
     * @see IPPacket::pack()
     * @see TCPPacket::pack()
     */
    virtual void pack();

    /**
     * @return The source port of this packet.
     */
    u_int16_t get_source_port() const;

    /**
     * Sets the  source port of this packet.
     *
     * @param port The source port to set.
     */
    void set_source_port(u_int16_t port);

    /**
     * @return The destination port of this packet.
     */
    u_int16_t get_destination_port() const;

    /**
     * Sets the destination port of this packet.
     *
     * @param port The destination port to set.
     */
    void set_destination_port(u_int16_t port);

    /**
     * Conveinence method which returns an AddressPort containing the source address and port.
     * @return Pointer to an AddressPort containing the source address and port.
     */
    AddressPort* get_source_address_port();

    /**
     * Conveinence method which returns an AddressPort containing the destination address and port.
     * @return Pointer to an AddressPort containing the destination address and port.
     */
    AddressPort* get_dest_address_port();

    /**
     * Returns the maximum number of bytes that this packet can store after the IP and WiFu headers.
     * For WiFu packet this is MTU minus the IP and WiFu header lengths.
     *
     * Child classes MUST override this method to ensure that they account for any underlying protocol headers.
     *
     * @return The maximum number of bytes that this packet can store after the IP and WiFu headers.
     *
     * @see IPPacket::max_data_length()
     */
    virtual int max_data_length();

    /**
     * @return A string representation of the fields in the IP and WiFu headers.
     *
     * Child classes MUST override this method to ensure that they account for any underlying protocol headers.
     * A child class MUST call IPPacket::to_s() (not WiFuPacket::to_s()) first and append the next protocol's header data (which MUST contain ports) on the next line.
     *
     * @see IPPacket::to_s()
     * @see WiFuPacket::to_s_format()
     */
    virtual gcstring to_s() const;

    /**
     * @return A string representing the names of the values produced by WiFuPacket::to_s().
     *
     * A child class MUST call IPPacket::to_s() (not WiFuPacket::to_s_format()) first and append the next protocol's header names (which must contain ports) on the next line.
     *
     * @see IPPacket::to_s_format()
     * @see WiFuPacket::to_s()
     */
    virtual gcstring to_s_format() const;

    /**
     * Compares this WiFuPacket with other and returns whether they are equal or not.
     * First, it checks the IPPacket header for equallity; second, it checks to ensure that it is a WiFuPacket, and finally it checks the WiFuPacket header.
     *
     * Child classes MUST override this method first calling the parent (this) function to ensure that they account for any underlying protocol equallity AND IP equallity.
     *
     * @param other IPPacket (which is actually a WiFuPacket) to compare with this WiFuPacket
     * @return True if every field in the IP and WiFu headers are equal in this WiFuPacket and other, false otherwise.
     *
     * @see IPPacket::operator==
     *
     */
    virtual bool operator ==(const IPPacket& other) const;

    /**
     * Compares this WiFuPacket with other and returns whether they are not equal.
     *
     * Child classes MUST override this method first calling the parent (this) function to ensure that they account for any underlying protocol inequallity AND IP inequallity.
     *
     * @param other IPPacket to compare with this WiFuPacket
     * @return the opposite of WiFuPacket::operator==
     */
    virtual bool operator !=(const IPPacket& other) const;

private:

    /**
     * Sets ports_ to immediately follow the IP header.
     * Calls IPPacket::set_ip_tot_length() and sets it to the IP header length plus the size of the ports.
     */
    void init();

    /**
     * Pointer to the posour and destination ports (of the underlying protocol).
     */
    struct wifu_packet_header* ports_;

    /**
     * Pointer to the source address and port.
     */
    AddressPort* source_;

    /**
     * Pointer to the destination address and port.
     */
    AddressPort* dest_;
};

#endif	/* _WIFUPACKET_H */

