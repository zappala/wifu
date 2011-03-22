/*
 * NetworkTrace.cc
 *
 *  Created on: Mar 17, 2011
 *      Author: erickson
 */

#include "NetworkTrace.h"

NetworkTrace::NetworkTrace() {}

void NetworkTrace::add_packet(WiFuPacket* packet) {
	packet_list_.push_back(packet);
}

void NetworkTrace::print_trace() {
	for (int index = 0; index < packet_list_.size(); ++index) {
		if (index == 0)
			cout << packet_list_.at(index)->to_s_format().c_str() << endl;
		cout << packet_list_.at(index)->to_s() << endl;
	}
}

bool NetworkTrace::operator ==(const NetworkTrace& other) const {
	if (packet_list_.size() == other.packet_list_.size())
		for (int index; index < packet_list_.size(); ++index)
			if (packet_list_.at(index) != other.packet_list_.at(index))
				return false;

	return true;
}
