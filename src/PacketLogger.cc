/*
 * PacketLogger.cc
 *
 *  Created on: Mar 8, 2011
 *      Author: erickson
 */

#include "PacketLogger.h"

PacketLogger& PacketLogger::getInstance() {
	static PacketLogger instance;
	return instance;
}

PacketLogger::~PacketLogger() {
	fileout.close();

	cout << "packet logger destructed" << endl;
}

void PacketLogger::log(WiFuPacket* packet) {
	setTime();
	packetHeader.ts_sec = time.tv_sec;
	packetHeader.ts_usec = time.tv_usec;
	packetHeader.incl_len = packet->get_ip_tot_length();
	packetHeader.orig_len = packet->get_ip_tot_length();

	fileout.write(reinterpret_cast<const char*>(&packetHeader), sizeof(pcapPacketHeader));
	fileout.write(reinterpret_cast<const char*>(packet->get_payload()), packet->get_ip_tot_length());
}

PacketLogger::PacketLogger() {
	fileName = "hello.pcap";
	fileout.open(fileName, ios::trunc | ios::binary);

	fillInFileHeader();
	writeFileHeader();

	cout << "packet logger constructed" << endl;
}

void PacketLogger::setTime() {
	gettimeofday(&time, NULL);
}

void PacketLogger::fillInFileHeader() {
	fileHeader.magic_number = 0xa1b2c3d4;
	fileHeader.version_major = 2;
	fileHeader.version_minor = 4;
	fileHeader.thiszone = 0;
	fileHeader.sigfigs = 0;
	fileHeader.snaplen = 65535;			//(2^16)
	fileHeader.network = 1;				//Ethernet
}

void PacketLogger::writeFileHeader() {
	fileout.write(reinterpret_cast<const char*>(&fileHeader), sizeof(pcapFileHeader));
	cout << "file header written" << endl;
}
