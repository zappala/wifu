/* 
 * File:   SimplePacket.h
 * Author: rbuck
 *
 * Created on December 17, 2010, 12:46 PM
 */

#ifndef _SIMPLEPACKET_H
#define	_SIMPLEPACKET_H

#define MSS 1500

struct SimpleHeader {
    u_int32_t id;
    u_int32_t ack;
};

class SimplePacket {
public:

    SimplePacket(unsigned char* data, size_t length, struct SimpleHeader* header) : payload_length_(length) {
        memcpy(buffer_ + get_header_length(), data, length);
        memcpy(buffer_, header, get_header_length());
    }

    SimplePacket(unsigned char* data, size_t length) : payload_length_(length - get_header_length()){
        memcpy(buffer_, data, length);
    }

    virtual ~SimplePacket() {

    }

    unsigned char* to_bytes() {
        return buffer_;
    }

    size_t get_header_length() {
        return sizeof (struct SimpleHeader);
    }

    size_t get_payload_length() {
        return payload_length_;
    }

    size_t get_total_length() {
        return get_header_length() + get_payload_length();
    }

    struct SimpleHeader* get_header() {
        return (struct SimpleHeader*) buffer_;
    }

    unsigned char* get_payload() {
        return buffer_ + get_header_length();
    }

private:
    size_t payload_length_;
    unsigned char buffer_[MSS];
};

#endif	/* _SIMPLEPACKET_H */

