/* 
 * File:   Packet.h
 * Author: rbuck
 *
 * Created on November 9, 2010, 10:11 AM
 */

#ifndef _PACKET_H
#define	_PACKET_H

#include "defines.h"
#include <string.h>
#include <assert.h>

using namespace std;

struct wifu_end_header {
    u_int16_t virtual_src_port;
    u_int16_t virtual_dest_port;
    u_int16_t packet_length;
};

#define END_HEADER_SIZE sizeof(struct wifu_end_header)

class Packet {
public:

    Packet( struct wifu_end_header* header,
            unsigned char* data,
            int data_length,
            string& source_address,
            string& dest_address)

            :   length_(header->packet_length),
                source_address_(source_address),
                dest_address_(dest_address) {

        memset(payload_, 0, PAYLOAD_SIZE);
        memcpy(payload_, header, END_HEADER_SIZE);

        unsigned char* ptr = payload_;
        ptr += sizeof (struct wifu_end_header);
        memcpy(ptr, data, data_length);

        assert(get_header()->packet_length == data_length + END_HEADER_SIZE);
    }

    /**
     * For use by the UDPInterface, the header is included in payload
     */
    Packet( unsigned char* payload,
            int payload_length,
            string& source_address,
            string& dest_address)

            :   length_(payload_length),
                source_address_(source_address),
                dest_address_(dest_address) {
        
        memset(payload_, 0, PAYLOAD_SIZE);
        memcpy(payload_, payload, payload_length);
    }

    virtual ~Packet() {

    }

    virtual unsigned char* to_bytes() {
        return payload_;
    }

    virtual int length() {
        return length_;
    }

    struct wifu_end_header* get_header() {
        return (struct wifu_end_header*) payload_;
    }

    string& get_source_address() {
        return source_address_;
    }

    string& get_dest_address() {
        return dest_address_;
    }

    u_int16_t& get_source_port() {
        return get_header()->virtual_src_port;
    }

    u_int16_t& get_dest_port() {
        return get_header()->virtual_dest_port;
    }

    unsigned char* get_data() {
        return payload_ + END_HEADER_SIZE;
    }

private:
    unsigned char payload_[PAYLOAD_SIZE];
    int length_;
    string source_address_;
    string dest_address_;
};

#endif	/* _PACKET_H */

