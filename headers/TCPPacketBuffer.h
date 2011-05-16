/* 
 * File:   TCPPacketBuffer.h
 * Author: rbuck
 *
 * Created on May 16, 2011, 2:34 PM
 */

#ifndef TCPPACKETBUFFER_H
#define	TCPPACKETBUFFER_H

#include "packet/TCPPacket.h"
#include "GarbageCollector.h"

class TCPPacketBuffer : public gc {
public:
    TCPPacketBuffer();
    virtual ~TCPPacketBuffer();
    
    int insert(TCPPacket* p);
    string get_continuous_data(u_int32_t sequence_number);

private:
    /**
     * Using a list allows us to use iterators which remain correct through modifications
     * http://www.cplusplus.com/reference/stl/list/insert/
     */
    list<TCPPacket*> buffer_;

};

#endif	/* TCPPACKETBUFFER_H */

