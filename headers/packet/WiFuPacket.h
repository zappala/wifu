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
    WiFuPacket();

    ~WiFuPacket();

    virtual unsigned char* get_data();
    virtual int get_data_length_bytes();

    virtual void set_data(unsigned char* data, int length);

    virtual void pack();

    /**
     * Gets the source port of this packet
     */
    u_int16_t get_source_port() const;

    /**
     * Sets the  source port of this packet
     */
    void set_source_port(u_int16_t port);

    /**
     * Gets the destination port of this packet
     */
    u_int16_t get_destination_port() const;

    /**
     * Sets the destination port of this packet
     */
    void set_destination_port(u_int16_t port);

    AddressPort* get_source_address_port();
    
    AddressPort* get_dest_address_port();

    

    virtual int max_data_length();

    virtual string to_s() const;
    virtual string to_s_format() const;

    virtual bool operator ==(const IPPacket& other) const;
    virtual bool operator !=(const IPPacket& other) const;

private:

    void init();
    
    struct wifu_packet_header* ports_;
    AddressPort* source_;
    AddressPort* dest_;
};

#endif	/* _WIFUPACKET_H */

