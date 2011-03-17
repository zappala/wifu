/*
 * NetworkTrace.h
 *
 *  Created on: Mar 17, 2011
 *      Author: erickson
 */

#ifndef NETWORKTRACE_H_
#define NETWORKTRACE_H_

#include "GarbageCollector.h"
#include "packet/WiFuPacket.h"
#include <vector>

using namespace std;

class NetworkTrace : public gc_cleanup {
public:
	NetworkTrace();

	void addPacket(WiFuPacket* packet);

private:
	vector<WiFuPacket*> packetList;

};

#endif /* NETWORKTRACE_H_ */
