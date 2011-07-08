/* 
 * File:   TCPPacketBuffer.h
 * Author: rbuck
 *
 * Created on May 16, 2011, 2:34 PM
 */

#ifndef TCPPACKETBUFFER_H
#define	TCPPACKETBUFFER_H

#include <list>
#include <algorithm>
#include "packet/TCPPacket.h"
#include "GarbageCollector.h"
#include "packet/TCPSequenceNumberComparator.h"
#include "Math.h"

typedef list<TCPPacket*, gc_allocator<TCPPacket*> > packet_buffer;

class TCPPacketBuffer : public gc {
public:
    TCPPacketBuffer();
    virtual ~TCPPacketBuffer();

    int insert(TCPPacket* p);
    TCPPacket* remove(TCPPacket* p);
    void get_continuous_data(u_int32_t sequence_number, gcstring& buffer);
    int size();
    u_int32_t get_first_sequence_number();
    void set_first_sequence_number(u_int32_t seq_num);

private:

    void mark_dirty();

    /**
     * We will only use the key (we basically use it as a tree)
     */
    packet_buffer buffer_;
    TCPSequenceNumberComparator comparator_;
    u_int32_t first_expected_sequence_number_;
    bool started_;

    int size_;
};

#endif	/* TCPPACKETBUFFER_H */

