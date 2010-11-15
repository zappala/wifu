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

using namespace std;


class Packet {
public:
    Packet() {

    }

    Packet(unsigned char * payload, int size) : size_(size) {
        memcpy(payload_, payload, size);
    }

    virtual ~Packet() {

    }

    virtual unsigned char * to_bytes() {
        return payload_;
    }

    virtual int size() {
        return size_;
    }    

private:
    unsigned char payload_[PAYLOAD_SIZE];
    int size_;
};

#endif	/* _PACKET_H */

