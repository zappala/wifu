/*
 * PacketLogger.cc
 *
 *  Created on: Mar 8, 2011
 *      Author: erickson
 */

#include "PacketLogger.h"

PacketLogger& PacketLogger::instance() {
    static PacketLogger instance;
    return instance;
}

PacketLogger::~PacketLogger() {
    close_log();
}

#include "packet/TCPPacket.h"
void PacketLogger::log(WiFuPacket* packet) {
    lock_.wait();

    set_time();
    fileout_.open(file_name_, ios::out | ios::app | ios::binary);
    if (fileout_.fail())
        throw IOError();

    const char fake_ethernet_header[14] = {0x00, 0x50, 0x56, 0xb7, 0x16, 0xc7, 0x00, 0x0f, 0xfe, 0xfe, 0x52, 0x17, 0x08, 0x00};

    packet_header_.ts_sec = time_.tv_sec;
    packet_header_.ts_usec = time_.tv_usec;
    packet_header_.incl_len = packet->get_ip_tot_length() + sizeof (fake_ethernet_header);
    packet_header_.orig_len = packet->get_ip_tot_length() + sizeof (fake_ethernet_header);

    TCPPacket* p = (TCPPacket*) packet;
    p->set_ip_protocol(6);
    p->calculate_and_set_ip_checksum();
    p->calculate_and_set_tcp_checksum();

    fileout_.write(reinterpret_cast<const char*> (&packet_header_), sizeof (PcapPacketHeader));
    fileout_.write(fake_ethernet_header, sizeof (fake_ethernet_header));
    fileout_.write(reinterpret_cast<const char*> (packet->get_payload()), packet->get_ip_tot_length());

    close_log();

    lock_.post();
}

PacketLogger::PacketLogger() {
    file_name_ = LOG_FILENAME;

    fileout_.open(file_name_, ios::out | ios::trunc | ios::binary);
    if (fileout_.fail())
        throw IOError();
    fill_in_file_header();
    write_file_header();
    close_log();

    lock_.init(1);
}

void PacketLogger::close_log() {
    if (fileout_.is_open())
        fileout_.close();
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
    file_header_.snaplen = USHRT_MAX;
    file_header_.network = 1; //Ethernet
}

void PacketLogger::write_file_header() {
    fileout_.write(reinterpret_cast<const char*> (&file_header_), sizeof (PcapFileHeader));
}
