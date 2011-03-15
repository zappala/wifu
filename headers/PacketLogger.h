/*
 * PacketLogger.h
 *
 *  Created on: Mar 8, 2011
 *      Author: erickson
 */

#ifndef PACKETLOGGER_H_
#define PACKETLOGGER_H_

#include <iostream>
#include <fstream>

#include "packet/WiFuPacket.h"
#include <stdint.h>
#include <sys/time.h>

using namespace std;

/**
 * These structs actually calls for guint32's and gint32 in glib.h
 * (as seen here: http://wiki.wireshark.org/Development/LibpcapFileFormat),
 * but this machine doesn't appear to have that header so I'm using uint32_t
 * instead.
 */
struct pcapFileHeader {
	uint32_t magic_number;   /* magic number */
	uint16_t version_major;  /* major version number */
	uint16_t version_minor;  /* minor version number */
	int32_t  thiszone;       /* GMT to local correction */
	uint32_t sigfigs;        /* accuracy of timestamps */
	uint32_t snaplen;        /* max length of captured packets, in octets */
	uint32_t network;        /* data link type */
};
struct pcapPacketHeader {
	uint32_t ts_sec;         /* timestamp seconds */
	uint32_t ts_usec;        /* timestamp microseconds */
	uint32_t incl_len;       /* number of octets of packet saved in file */
	uint32_t orig_len;       /* actual length of packet */
};


class PacketLogger {
public:
	static PacketLogger& getInstance();

	~PacketLogger();

	void log(WiFuPacket* packet);

private:
	PacketLogger();

	void setTime();

	void fillInFileHeader();

	void writeFileHeader();

	const char* fileName;
	ofstream fileout;
	pcapFileHeader fileHeader;
	pcapPacketHeader packetHeader;
	timeval time;
};

#endif /* PACKETLOGGER_H_ */
