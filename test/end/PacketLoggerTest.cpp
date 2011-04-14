/*
 * PacketLoggerTest.cpp
 *
 *  Created on: Mar 8, 2011
 *      Author: erickson
 */

#include "gtest/gtest.h"
#include "packet/TCPPacket.h"
#include "defines.h"

/*
 * Only the following two need to be included.
 */
#include "PacketLogger.h"
#include "PacketLogReader.h"

using namespace std;

/*
 * Convenience only.
 */
#define logger PacketLogger::get_instance()

namespace {

	TEST(PacketLogger, test) {
		TCPPacket* packet1 = new TCPPacket();	//be sure to call 'new'
		packet1->set_ip_protocol(SIMPLE_TCP);	//very important
		unsigned char data1[] = "hello";		//whatever data, could be blank ("") too I suppose
		packet1->set_data(data1, sizeof(data1));	//set_data has to be called so that ip_tot is updated

		/**
		 * use this to make not equal traces
		 *
		TCPPacket* packet2 = new TCPPacket();
		packet2->set_ip_protocol(SIMPLE_TCP);
		unsigned char data2[] = "hello";
		packet2->set_data(data2, sizeof(data2));
		packet2->set_tcp_syn(true);
		packet2->set_tcp_fin(true);
		 */

		/*
		 * These should only be called in the RawSocket* files (which they are right now).
		 * They are only called here so that a fake network trace is written to the log.
		 */
		logger.log(packet1);
		/**
		 * use these to make not equal traces
		 *
		logger.log(packet2);
		logger.log(packet1);
		logger.log(packet2);
		 */

		NetworkTrace expected;	//the sent and received packets that you expect from the daemon's perspective
		expected.add_packet(packet1);
		/**
		 * use these to make not equal traces
		 *
		expected.add_packet(packet1);
		expected.add_packet(packet1);
		expected.add_packet(packet2);
		 */

		PacketLogReader log_reader(LOG_FILENAME);
		NetworkTrace* actual = log_reader.get_trace();

		/*
		 * The order is important on the streaming.
		 * The object that has "get_packet_trace" called on it assumes that it is the EXPECTED packet trace.
		 * 'actual' is dereferenced so that the operator== is called correctly.
		 */
		ASSERT_EQ(expected, *actual) << expected.get_packet_trace(*actual); //
	}

}
