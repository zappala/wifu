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
    flush();
}

void PacketLogger::log(WiFuPacket* packet) {
    lock_.wait();
    items_.push_back(new PacketLoggerItem(packet));
    bool should_flush = items_.size() >= flush_count_;
    lock_.post();


    if (should_flush) {
        flush();
    }
}

void PacketLogger::flush() {
    lock_.wait();
    fileout_.open(file_name_, ios::out | ios::app | ios::binary);
    if (fileout_.fail()) {
        throw IOError();
    }

    assert(fileout_.is_open());

    const char fake_ethernet_header[FAKE_ETHERNET_HEADER_SIZE] = {0x00, 0x50, 0x56, 0xb7, 0x16, 0xc7, 0x00, 0x0f, 0xfe, 0xfe, 0x52, 0x17, 0x08, 0x00};

    while (!items_.empty()) {
        PacketLoggerItem* item = items_.front();
        items_.pop_front();

        PcapPacketHeader packet_header;
        packet_header.ts_sec = item->get_time()->tv_sec;
        packet_header.ts_usec = item->get_time()->tv_usec;
        packet_header.incl_len = item->get_packet()->get_ip_tot_length() + FAKE_ETHERNET_HEADER_SIZE;
        packet_header.orig_len = item->get_packet()->get_ip_tot_length() + FAKE_ETHERNET_HEADER_SIZE;

        fileout_.write(reinterpret_cast<const char*> (&packet_header), sizeof (PcapPacketHeader));
        fileout_.write(fake_ethernet_header, sizeof (fake_ethernet_header));
        fileout_.write(reinterpret_cast<const char*> (item->get_packet()->get_payload()), item->get_packet()->get_ip_tot_length());
    }

    close_log();
    dispatch_timeout();
    lock_.post();
}

PacketLogger::PacketLogger() : Module(), flush_count_(1), flush_seconds_(1), flush_nanoseconds_(0), timeout_(0), fake_socket_(new Socket(0,0,0)) {
    // TODO: we can remove this call to init() as we call reset() in main().
    // However, this may cause some tests to break.
    // By not removing it, we are simply creating the file one extra time.
    init();
}

void PacketLogger::set_flush_threshold(int count) {
    flush_count_ = count;

}

void PacketLogger::set_flush_timeout(double timeout) {
    flush_nanoseconds_ = modf(timeout, &flush_seconds_) * NANOSECONDS_IN_SECONDS;
}

void PacketLogger::reset() {
    close_log();
    init();
    dispatch_timeout();
}

void PacketLogger::imodule_timer_fired(Event* e) {
    TimerFiredEvent* event = (TimerFiredEvent*) e;
    if (timeout_ == event->get_timeout_event()) {
        flush();
    }
}

void PacketLogger::close_log() {
    if (fileout_.is_open()) {
        fileout_.close();
    }
}

void PacketLogger::write_file_header() {
    PcapFileHeader file_header;
    file_header.magic_number = 0xa1b2c3d4;
    file_header.version_major = 2;
    file_header.version_minor = 4;
    file_header.thiszone = 0;
    file_header.sigfigs = 0;
    file_header.snaplen = USHRT_MAX;
    file_header.network = 1; //Ethernet
    fileout_.write(reinterpret_cast<const char*> (&file_header), sizeof (PcapFileHeader));
}

void PacketLogger::init() {
    file_name_ = LOG_FILENAME;

    fileout_.open(file_name_, ios::out | ios::trunc | ios::binary);
    if (fileout_.fail()) {
        throw IOError();
    }
    write_file_header();
    close_log();

    lock_.init(1);
}

void PacketLogger::dispatch_timeout() {
    timeout_ = new TimeoutEvent(fake_socket_, flush_seconds_, flush_nanoseconds_);
    dispatch(timeout_);
}
