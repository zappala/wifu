/*
 * PacketLoggerTest.cpp
 *
 *  Created on: Mar 8, 2011
 *      Author: erickson
 */

#include "gtest/gtest.h"
#include "PacketLogger.h"
#include "PacketLogReader.h"
#include "packet/TCPPacket.h"

#include <iostream>

using namespace std;

#define logger PacketLogger::get_instance()

namespace {

	TEST(PacketLogger, test) {
		logger;
		TCPPacket* packet = new TCPPacket();
		unsigned char data[] = "hello";
		packet->set_data(data, sizeof(data));

		logger.log(packet);
		logger.log(packet);
		logger.close_log();

		NetworkTrace expected;
		expected.add_packet(packet);
		expected.add_packet(packet);
		expected.add_packet(packet);

		PacketLogReader reader(LOG_FILENAME);
		NetworkTrace* trace = reader.get_trace();

		ASSERT_EQ(expected, *trace) << expected.get_TCPPacket_trace(*trace);
	}

}
