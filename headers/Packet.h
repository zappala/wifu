/* 
 * File:   Packet.h
 * Author: rbuck
 *
 * Created on November 9, 2010, 10:11 AM
 */

#ifndef _PACKET_H
#define	_PACKET_H

#include "AddressPort.h"
#include "defines.h"
#include <string.h>
#include <assert.h>

using namespace std;

struct wifu_end_header {
    u_int16_t virtual_src_port;
    u_int16_t virtual_dest_port;
};

#define END_HEADER_SIZE sizeof(struct wifu_end_header)

class Packet {
public:

    Packet( AddressPort* source,
            AddressPort* dest,
            unsigned char* data,
            int data_length)
            :   length_(data_length),
                source_(source),
                destination_(dest) {

        memset(payload_, 0, PAYLOAD_SIZE);

        struct wifu_end_header header;
        header.virtual_dest_port = dest->get_port();
        header.virtual_src_port = source->get_port();        
        memcpy(payload_, &header, END_HEADER_SIZE);

        unsigned char* ptr = payload_;
        ptr += END_HEADER_SIZE;
        memcpy(ptr, data, length_);
    }

    virtual ~Packet() {

    }

    virtual unsigned char* to_bytes() {
        return payload_;
    }

    virtual int length() {
        return length_;
    }

    AddressPort* get_source() {
        return source_;
    }

    AddressPort* get_destination() {
        return destination_;
    }

    unsigned char* get_data() {
        return payload_ + END_HEADER_SIZE;
    }

private:
    unsigned char payload_[PAYLOAD_SIZE];
    int length_;

    AddressPort* source_;
    AddressPort* destination_;
};

#endif	/* _PACKET_H */

