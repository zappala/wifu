/* 
 * File:   UDPPacket.h
 * Author: leer
 *
 * Created on June 22, 2011, 10:48 AM
 */

#ifndef _UDPPACKET_H
#define	_UDPPACKET_H

#include "packet/WiFuPacket.h"
#include "exceptions/IllegalStateException.h"
#include "defines.h"
#include <netinet/udp.h>

using namespace std;

class UDPPacket : public WiFuPacket{
public:
    UDPPacket();
    UDPPacket(const UDPPacket& orig);
    virtual ~UDPPacket();

    int max_data_length();

    u_int16_t get_udp_checksum() const;
    void set_udp_checksum(u_int16_t checksum);

    void calculate_and_set_udp_checksum();
    bool is_valid_udp_checksum();

    virtual unsigned char* get_data();
    virtual int get_data_length_bytes();

    virtual u_int16_t get_udp_length_bytes() const;
    virtual void set_udp_length_bytes(u_int16_t newlen);

    virtual void set_data(unsigned char* data, int length);

    virtual gcstring to_s() const;
    virtual gcstring to_s_format() const;

    virtual bool operator ==(const IPPacket& other) const;
    virtual bool operator !=(const IPPacket& other) const;
private:

    void init();

    struct udphdr* udp_;
    bool data_set_;
};

#endif	/* _UDPPACKET_H */

