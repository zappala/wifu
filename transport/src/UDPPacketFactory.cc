/* 
 * File:   UDPPacketFactory.cc
 * Author: leer
 * 
 * Created on June 27, 2011, 11:15 AM
 */

#include "UDPPacketFactory.h"
#include "packet/UDPPacket.h"

WiFuPacket* UDPPacketFactory::create() {
    return new UDPPacket();
}

