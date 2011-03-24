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

	string get_packet_trace(NetworkTrace& other) const;

	int get_length() const;

	bool operator ==(const NetworkTrace& other) const;

private:
	bool equal_size_traces(const NetworkTrace& one, const NetworkTrace& two) const;

	string get_packet_string(int packet_number, WiFuPacket* packet) const;

	string get_nonequal_packets_string(int packet_number, WiFuPacket* one, WiFuPacket* two) const;

	vector<WiFuPacket*> packet_list_;

};

ostream& operator <<(ostream& os, const NetworkTrace& trace);

#endif /* NETWORKTRACE_H_ */
