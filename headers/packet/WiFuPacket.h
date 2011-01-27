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

    // Gets the source port of this packet
    u_int16_t get_source_port();

    // Sets the  source port of this packet
    void set_source_port(u_int16_t port);

    // Gets the destination port of this packet
    u_int16_t get_destination_port();

    // Sets the destination port of this packet
    void set_destination_port(u_int16_t port);

    AddressPort* get_source_address_port();
    
    AddressPort* get_dest_address_port();

private:
    struct wifu_packet_header* ports_;
};

#endif	/* _WIFUPACKET_H */

