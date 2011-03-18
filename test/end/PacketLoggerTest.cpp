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

//namespace {
//
//	TEST(PacketLogger, test) {
//		logger;
//		TCPPacket packet;
//		unsigned char data[] = "hello";
//		packet.set_data(data, sizeof(data));
//
////		cout << "default length " << packet.get_ip_tot_length() << endl;
////		char data[] = "hello";
////		packet.set_data((unsigned char*)data, sizeof(data));
//
////		packet.set_ip_tot_length(
////				packet.get_ip_header_length_bytes() +
////				sizeof(wifu_packet_header) +
////				packet.get_tcp_header_length_bytes() +
////				(packet.get_data_length_bytes() - sizeof(wifu_packet_header)) +
////				sizeof(data));
//
////		cout << "my math " <<
////				packet.get_ip_header_length_bytes() +
////				sizeof(wifu_packet_header) +
////				packet.get_tcp_header_length_bytes() +
////				(packet.get_data_length_bytes() - sizeof(wifu_packet_header)) +
////				sizeof(data) << endl;
////		cout << "data_length_bytes " << packet.get_data_length_bytes() << endl <<
////				"destination_port " << packet.get_destination_port() << endl <<
////				"ip_length " << packet.get_ip_header_length_words() << endl <<
////				"ip_length_bytes " << packet.get_ip_header_length_bytes() << endl <<
////				"length_is_set " << packet.length_is_set() << endl <<
////				"ip_payload_length" << packet.get_ip_max_payload_length_bytes();
//
//		logger.log(&packet);
//		logger.log(&packet);
//		logger.close_log();
//
//		PacketLogReader reader(LOG_FILENAME);
//		reader.get_trace();
//	}
//
//}
