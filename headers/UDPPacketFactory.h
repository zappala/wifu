/* 
 * File:   UDPPacketFactory.h
 * Author: leer
 *
 * Created on June 27, 2011, 11:15 AM
 */

#ifndef _UDPPACKETFACTORY_H
#define	_UDPPACKETFACTORY_H

#include "PacketFactory.h"
#include "packet/TCPPacket.h"

class UDPPacketFactory : public PacketFactory {
    WiFuPacket* create();
};

#endif	/* _UDPPACKETFACTORY_H */

