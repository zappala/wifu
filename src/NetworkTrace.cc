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

void NetworkTrace::print_trace() const {
	for (int index = 0; index < packet_list_.size(); ++index) {
		if (index == 0)
			cout << packet_list_.at(index)->to_s_format().c_str() << endl;
		cout << packet_list_.at(index)->to_s() << endl;
	}
}

string NetworkTrace::get_TCPPacket_trace(NetworkTrace& other) const {
	stringstream stream;
	stream << endl <<
			  "*********** Packet Trace ***********" << endl <<
			  "Printing traces starting from time 0" << endl <<
			  endl;
	if (!equal_size_traces(*this, other)) {
		bool this_is_shorter = packet_list_.size() < other.packet_list_.size();
		int shorter_max;
		if (this_is_shorter)
			shorter_max = packet_list_.size();
		else
			shorter_max = other.packet_list_.size();

		int first_nonequal_index = -1;
		for (int index = 0; index < shorter_max; ++index)
		{
			if (index == 0)
				stream << ((TCPPacket*)packet_list_.at(0))->to_s_format() << endl;

			stream << "WHAT THE? " << ((*(packet_list_.at(index))) == (*(other.packet_list_.at(index)))) << endl;

			//section to delete
//			stream << print_TCPPacket(packet_list_.at(index)) << endl;
			stream << print_TCPPacket(other.packet_list_.at(index)) << endl;
			//

			if ( (*(packet_list_.at(index))) == (*(other.packet_list_.at(index))) && first_nonequal_index == -1)
				stream << print_TCPPacket(packet_list_.at(index)) << endl;
//			else if (first_nonequal_index == -1)
//			{
//				stream << print_nonequal_TCPPackets(packet_list_.at(index), other.packet_list_.at(index)) << endl;
//				first_nonequal_index = index;
//			}
//			else
//			{
//				if (first_nonequal_index == index + 1)
//					stream << "Successive packets will not be checked for equality" << endl;
//				stream << "--" << endl <<
//						  print_TCPPacket(packet_list_.at(index)) << endl <<
//						  print_TCPPacket(other.packet_list_.at(index)) << endl;
//			}
		}

//		vector<WiFuPacket*> shorter_list = this_is_shorter ? packet_list_ : other.packet_list_;
//		vector<WiFuPacket*> longer_list = this_is_shorter ? packet_list_ : other.packet_list_;
//		for (int index = shorter_list.size(); index < longer_list.size(); ++index)
//		{
//			if (index == shorter_list.size())
//			{
//				stream << "--" << endl;
//				if (this_is_shorter)
//					stream << "Expected packets:" << endl;
//				else
//					stream << "Actual packets:" << endl;
//			}
//
//			stream << print_TCPPacket(longer_list.at(index)) << endl;
//		}
	}

	return stream.str();
}

int NetworkTrace::get_length() const {
	return packet_list_.size();
}

bool NetworkTrace::operator ==(const NetworkTrace& other) const {
	if (equal_size_traces(*this, other)) {
		for (int index; index < packet_list_.size(); ++index)
			if (packet_list_.at(index) != other.packet_list_.at(index))
				return false;
		return true;
	}
	else
		return false;
}

bool NetworkTrace::equal_size_traces(const NetworkTrace& one, const NetworkTrace& two) const {
	return one.packet_list_.size() == two.packet_list_.size();
}

string NetworkTrace::print_TCPPacket(WiFuPacket* packet) const {
	return static_cast<TCPPacket*>(packet)->to_s();
}

string NetworkTrace::print_nonequal_TCPPackets(WiFuPacket* one, WiFuPacket* two) const {
	stringstream stream;
	stream << "== First non-equal TCPPackets ==" << endl <<
			  "Expected:" << endl <<
	          print_TCPPacket(one) << endl <<
	          "Actual:" << endl <<
	          print_TCPPacket(two) << endl <<
	          "================================" << endl;
	return stream.str();
}

ostream& operator <<(ostream& os, const NetworkTrace& packet) {
	return os << "Size of trace: " << packet.get_length();
}
