/* 
 * File:   TCPPacketFactory.h
 * Author: rbuck
 *
 * Created on January 28, 2011, 2:06 PM
 */

#ifndef _TCPPACKETFACTORY_H
#define	_TCPPACKETFACTORY_H

#include "PacketFactory.h"
#include "packet/TCPPacket.h"
#include <vector>

class TCPPacketFactory : public PacketFactory {
public:
    
    TCPPacketFactory();

    WiFuPacket* create();

private:
    vector<TCPPacket*, gc_allocator<TCPPacket*> > packets_;
    WiFuPacket* p_;
};

#endif	/* _TCPPACKETFACTORY_H */

