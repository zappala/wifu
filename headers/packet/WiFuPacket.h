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

struct wifu_packet_header {
    u_int16_t sport;
    u_int16_t dport;
};

class WiFuPacket : public IPPacket {
public:
    WiFuPacket();
    WiFuPacket(IPPacket&);
    WiFuPacket(unsigned char* buffer, int length);
    ~WiFuPacket();

    virtual unsigned char* get_data();
    virtual int get_data_length_bytes();

    virtual void set_data(unsigned char* data, int length);

    /**
     * Gets the source port of this packet
     */
    u_int16_t get_source_port();

    /**
     * Sets the  source port of this packet
     */
    void set_source_port(u_int16_t port);

    /**
     * Gets the destination port of this packet
     */
    u_int16_t get_destination_port();

    /**
     * Sets the destination port of this packet
     */
    void set_destination_port(u_int16_t port);

    AddressPort* get_source_address_port();
    
    AddressPort* get_dest_address_port();

    void init();

    virtual int max_data_length();

    virtual string to_s();
    virtual string to_s_format();

private:
    struct wifu_packet_header* ports_;
    AddressPort* source_;
    AddressPort* dest_;
};

#endif	/* _WIFUPACKET_H */

