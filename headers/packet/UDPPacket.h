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

class UDPPacket : public WiFuPacket{
public:
    UDPPacket();
    UDPPacket(const UDPPacket& orig);
    virtual ~UDPPacket();

    int max_data_length();

    u_int16_t get_udp_checksum() const;
    void set_udp_checksum(u_int16_t checksum);

    virtual unsigned char* get_data();
    virtual int get_length_bytes();

    virtual void set_data(unsigned char* data, int length);

    virtual string to_s() const;
    virtual string to_s_format() const;

    virtual bool operator ==(const IPPacket& other) const;
    virtual bool operator !=(const IPPacket& other) const;
private:

    void init();

    struct udphdr* udp_;
    bool data_set_;
};

#endif	/* _UDPPACKET_H */

