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

string NetworkTrace::get_packet_trace(NetworkTrace& other) const {
	stringstream stream;
	stream << endl <<
			  "*********** Packet Trace ***********" << endl;

	int packet_number = 1;
	int first_nonequal_index = -1;
	int packets_in_shorter_trace = packet_list_.size() < other.packet_list_.size() ? packet_list_.size() : other.packet_list_.size();

	for (int index = 0; index < packets_in_shorter_trace; ++index, ++packet_number)
	{
		if (index == 0)
			stream << packet_list_.at(0)->to_s_format() << endl;

		if ( *(packet_list_.at(index)) == *(other.packet_list_.at(index)) && first_nonequal_index == -1)
		{
			if (index == 0)
				stream << "== Identical packets" << endl;

			stream << get_packet_string(packet_number, packet_list_.at(index)) << endl;
		}
		else if (first_nonequal_index == -1)
		{
			stream << get_nonequal_packets_string(packet_number, packet_list_.at(index), other.packet_list_.at(index)) << endl;
			first_nonequal_index = index;
		}
		else
		{
			if (index == first_nonequal_index + 1)
				stream << endl << "*Successive packets will not be checked for equality";

			stream << endl <<
					  "Expected:" << endl <<
					  get_packet_string(packet_number, packet_list_.at(index)) << endl <<
					  "Actual:" << endl <<
					  get_packet_string(packet_number, other.packet_list_.at(index)) << endl;
		}
	}


	if (!equal_size_traces(*this, other))
	{
		bool this_is_shorter = packet_list_.size() < other.packet_list_.size();
		vector<WiFuPacket*> shorter_list = this_is_shorter ? packet_list_ : other.packet_list_;
		vector<WiFuPacket*> longer_list = this_is_shorter ? other.packet_list_ : packet_list_;

		for (int index = packets_in_shorter_trace; index < longer_list.size(); ++index, ++packet_number)
		{
			if (index == packets_in_shorter_trace)
			{
				if (packet_number == 1)
					stream << longer_list.at(index)->to_s_format();

				stream << endl <<
						  "++ " <<
						  (this_is_shorter ? "Extra actual packets" : "Extra expected packets") << endl;
			}

			stream << get_packet_string(packet_number, longer_list.at(index)) << endl;
		}
	}

	return stream.str();
}

int NetworkTrace::get_length() const {
	return packet_list_.size();
}

bool NetworkTrace::operator ==(const NetworkTrace& other) const {
	if (equal_size_traces(*this, other)) {
		for (int index = 0; index < packet_list_.size(); ++index)
			if (*packet_list_.at(index) != *other.packet_list_.at(index))
				return false;
		return true;
	}
	else
		return false;
}

bool NetworkTrace::equal_size_traces(const NetworkTrace& one, const NetworkTrace& two) const {
	return one.packet_list_.size() == two.packet_list_.size();
}

string NetworkTrace::get_packet_string(int packet_number, WiFuPacket* packet) const {
	stringstream stream;
	stream << packet_number << endl <<
			  packet->to_s();
	return stream.str();
}

string NetworkTrace::get_nonequal_packets_string(int packet_number, WiFuPacket* one, WiFuPacket* two) const {
	stringstream stream;
	if (packet_number > 1)
		stream << endl;
	stream << "!= First non-equal packets" << endl <<
			  "Expected:" << endl <<
	          get_packet_string(packet_number, one) << endl <<
	          "Actual:" << endl <<
	          get_packet_string(packet_number, two);
	return stream.str();
}

ostream& operator <<(ostream& os, const NetworkTrace& trace) {
	return os << "Size of trace: " << trace.get_length();
}
