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

class TCPPacketFactory : public PacketFactory {
    WiFuPacket* create();
};

#endif	/* _TCPPACKETFACTORY_H */

