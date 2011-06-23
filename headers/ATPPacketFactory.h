/*
 * ATPPacketFactory.h
 *
 *  Created on: Jun 15, 2011
 *      Author: philipbl
 */

#ifndef ATPPACKETFACTORY_H_
#define ATPPACKETFACTORY_H_

#include "PacketFactory.h"
#include "packet/ATPPacket.h"

class ATPPacketFactory : public PacketFactory  {
    WiFuPacket* create();
};

#endif /* ATPPACKETFACTORY_H_ */
