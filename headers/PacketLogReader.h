/*
 * PacketLogReader.h
 *
 *  Created on: Mar 17, 2011
 *      Author: erickson
 */

#ifndef LOGREADER_H_
#define LOGREADER_H_

#include "NetworkTrace.h"
#include "PacketLogger.h"
#include <fstream>
#include <iostream>

using namespace std;

class PacketLogReader {
public:
	PacketLogReader(const char* fn);

	NetworkTrace* get_trace();

private:
	void open_log();
	void close_log();

	int get_log_length();

	void read_in_file_header();
	PcapFileHeader* get_file_header_pointer();

	void read_in_packet_header();
	PcapPacketHeader* get_packet_header_pointer();

	bool more_packets_to_read();

	const char* filename_;
	ifstream filein_;
	int log_length_;

	char pcap_file_header_buffer_[sizeof(PcapFileHeader)];
	char pcap_packet_header_buffer_[sizeof(PcapPacketHeader)];

};

#endif /* LOGREADER_H_ */
