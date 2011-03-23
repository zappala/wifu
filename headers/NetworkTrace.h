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
#include <vector>

using namespace std;

class NetworkTrace : public gc_cleanup {
public:
	NetworkTrace();

	void add_packet(WiFuPacket* packet);

	void print_trace() const;

	string get_TCPPacket_trace(NetworkTrace& other) const;

	int get_length() const;

	bool operator ==(const NetworkTrace& other) const;

private:
	bool equal_size_traces(const NetworkTrace& one, const NetworkTrace& two) const;

	string print_TCPPacket(WiFuPacket* packet) const;

	string print_nonequal_TCPPackets(WiFuPacket* one, WiFuPacket* two) const;

	vector<WiFuPacket*> packet_list_;

};

ostream& operator <<(ostream& os, const NetworkTrace& packet);

#endif /* NETWORKTRACE_H_ */
