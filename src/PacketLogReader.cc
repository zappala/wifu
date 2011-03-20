/*
 * PacketLogReader.cc
 *
 *  Created on: Mar 17, 2011
 *      Author: erickson
 */

#include "PacketLogReader.h"

PacketLogReader::PacketLogReader(const char* fn) {
	filename_ = fn;
}

NetworkTrace* PacketLogReader::get_trace() {
	open_log();

	NetworkTrace* trace = new NetworkTrace();
	log_length_ = get_log_length();

	if (log_length_ > sizeof(PcapFileHeader)) {
//		read_in_file_header();
		while (more_packets_to_read()) {
			read_in_packet_header();

			WiFuPacket packet;
			filein_.read((char*)packet.get_payload(), get_packet_header_pointer()->incl_len);

			cout << "read " << filein_.gcount() << endl;
			trace->add_packet(&packet);
			cout << packet.to_s_format() << endl << packet.to_s() << endl;
		}
	}

	close_log();

	return trace;
}

void PacketLogReader::open_log() {
	filein_.open(filename_, ios::in | ios::binary);
}

void PacketLogReader::close_log() {
	if (filein_.is_open())
		filein_.close();
}

int PacketLogReader::get_log_length() {
	int length;

	filein_.seekg(0, ios::end);
	length = filein_.tellg();
	filein_.seekg(0, ios::beg);

	return length;
}

void PacketLogReader::read_in_file_header() {
	filein_.read(pcap_file_header_buffer_, sizeof(PcapFileHeader));
}

PcapFileHeader* PacketLogReader::get_file_header_pointer() {
	return (PcapFileHeader*)pcap_file_header_buffer_;
}

void PacketLogReader::read_in_packet_header() {
	filein_.read(pcap_packet_header_buffer_, sizeof(PcapFileHeader));
}

PcapPacketHeader* PacketLogReader::get_packet_header_pointer() {
	return (PcapPacketHeader*)pcap_packet_header_buffer_;
}

bool PacketLogReader::more_packets_to_read() {
	cout << endl << "eof " << filein_.eof() << endl
	     << "failbit " << filein_.fail() << endl
	     << "bad " << filein_.bad() << endl;
	if (log_length_ - filein_.tellg() > sizeof(PcapPacketHeader) &&
		!filein_.eof() &&
		!filein_.fail() &&
		!filein_.bad() )
		return true;
	else
		return false;
}
