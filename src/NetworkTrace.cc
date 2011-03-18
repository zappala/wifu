/*
 * NetworkTrace.cc
 *
 *  Created on: Mar 17, 2011
 *      Author: erickson
 */

#include "NetworkTrace.h"

NetworkTrace::NetworkTrace() {

}

void NetworkTrace::addPacket(WiFuPacket* packet) {
	packetList.push_back(packet);
}
