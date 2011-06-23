/*
 * ATPPacketFactory.cpp
 *
 *  Created on: Jun 15, 2011
 *      Author: philipbl
 */

#include "../headers/ATPPacketFactory.h"

WiFuPacket* ATPPacketFactory::create() {
    return new ATPPacket();
}

