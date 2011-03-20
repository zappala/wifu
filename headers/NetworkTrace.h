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

	void add_packet(WiFuPacket* packet);

	void print_trace();

	bool operator ==(const NetworkTrace& other) const;

private:
	vector<WiFuPacket*> packet_list_;

};

#endif /* NETWORKTRACE_H_ */
