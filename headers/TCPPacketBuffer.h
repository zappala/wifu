/* 
 * File:   TCPPacketBuffer.h
 * Author: rbuck
 *
 * Created on May 16, 2011, 2:34 PM
 */

#ifndef TCPPACKETBUFFER_H
#define	TCPPACKETBUFFER_H

#include <map>
#include "packet/TCPPacket.h"
#include "GarbageCollector.h"
#include "packet/TCPSequenceNumberComparator.h"
#include "Math.h"

typedef map<TCPPacket*, char, TCPSequenceNumberComparator> packet_buffer;

class TCPPacketBuffer : public gc {
public:
    TCPPacketBuffer();
    virtual ~TCPPacketBuffer();

    int insert(TCPPacket* p);
    void get_continuous_data(u_int32_t sequence_number, string& buffer);
    int size();

private:

    void mark_dirty();

    /**
     * We will only use the key (we basically use it as a tree)
     */
    packet_buffer buffer_;

    int size_;
};

#endif	/* TCPPACKETBUFFER_H */

