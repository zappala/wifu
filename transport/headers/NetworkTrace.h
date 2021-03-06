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
#include "packet/TCPPacket.h"
#include <sstream>
#include <vector>

using namespace std;

class NetworkTrace : public gc_cleanup {
public:
	NetworkTrace();

	void add_packet(WiFuPacket* packet);

	/**
	 * This method assumes that this object is the expected trace while 'other'
	 * is what was actually was logged
	 */
	gcstring get_packet_trace(NetworkTrace& other) const;

	int get_length() const;

	bool operator ==(const NetworkTrace& other) const;

private:
	bool equal_size_traces(const NetworkTrace& one, const NetworkTrace& two) const;

	gcstring get_packet_string(int packet_number, WiFuPacket* packet) const;

	gcstring get_nonequal_packets_string(int packet_number, WiFuPacket* one, WiFuPacket* two) const;

	vector<WiFuPacket*, gc_allocator<WiFuPacket*> > packet_list_;

};

ostream& operator <<(ostream& os, const NetworkTrace& trace);

#endif /* NETWORKTRACE_H_ */
