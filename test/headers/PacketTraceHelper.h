/* 
 * File:   PacketTraceHelper.h
 * Author: rbuck
 *
 * Created on April 18, 2011, 12:16 PM
 */

#ifndef PACKETTRACEHELPER_H
#define	PACKETTRACEHELPER_H

#include "gtest/gtest.h"
#include "../headers/PacketLogReader.h"
#include "../headers/packet/TCPPacket.h"
#include "../headers/defines.h"
#include "../headers/packet/TCPTimestampOption.h"

#include <list>

// compares expected with the actual
void compare_traces(NetworkTrace& expected);

// compares all traces inside expected against actual, if any are equal the test passes
void compare_traces(list<NetworkTrace*>& expected);

TCPPacket* get_base_tcp_packet(int protocol);
TCPPacket* get_base_tcp_packet_ts(int protocol);
TCPPacket* get_syn(int protocol);
TCPPacket* get_synack(int protocol);
TCPPacket* get_ack(int protocol);

#endif	/* PACKETTRACEHELPER_H */

