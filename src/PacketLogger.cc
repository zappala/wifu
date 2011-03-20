/*
 * PacketLogger.cc
 *
 *  Created on: Mar 8, 2011
 *      Author: erickson
 */

#include "PacketLogger.h"

PacketLogger& PacketLogger::get_instance() {
	static PacketLogger instance;
	return instance;
}

PacketLogger::~PacketLogger() {
	close_log();
}

void PacketLogger::log(WiFuPacket* packet) {
	set_time();
	packet_header_.ts_sec = time_.tv_sec;
	packet_header_.ts_usec = time_.tv_usec;
	packet_header_.incl_len = packet->get_ip_tot_length();
	packet_header_.orig_len = packet->get_ip_tot_length();

	fileout_.write(reinterpret_cast<const char*>(&packet_header_), sizeof(PcapPacketHeader));
	fileout_.write(reinterpret_cast<const char*>(packet->get_payload()), packet->get_ip_tot_length());
}

void PacketLogger::close_log() {
	if (fileout_.is_open())
		fileout_.close();
}

PacketLogger::PacketLogger() {
	file_name_ = LOG_FILENAME;
	fileout_.open(file_name_, ios::out | ios::trunc | ios::binary);

	fill_in_file_header();
//	write_file_header();
}

void PacketLogger::set_time() {
	gettimeofday(&time_, NULL);
}

void PacketLogger::fill_in_file_header() {
	file_header_.magic_number = 0xa1b2c3d4;
	file_header_.version_major = 2;
	file_header_.version_minor = 4;
	file_header_.thiszone = 0;
	file_header_.sigfigs = 0;
	file_header_.snaplen = MTU;
	file_header_.network = 1;		//Ethernet
}

void PacketLogger::write_file_header() {
	fileout_.write(reinterpret_cast<const char*>(&file_header_), sizeof(PcapFileHeader));
	cout << "file header written" << endl;
}
