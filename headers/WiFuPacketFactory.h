/* 
 * File:   WiFuPacketFactory.h
 * Author: rbuck
 *
 * Created on January 28, 2011, 2:08 PM
 */

#ifndef _WIFUPACKETFACTORY_H
#define	_WIFUPACKETFACTORY_H

#include "PacketFactory.h"

class WiFuPacketFactory : public PacketFactory{
    WiFuPacket* create() {
        return new WiFuPacket();
    }
};

#endif	/* _WIFUPACKETFACTORY_H */

