/* 
 * File:   Packet.h
 * Author: rbuck
 *
 * Created on November 9, 2010, 10:11 AM
 */

#ifndef _PACKET_H
#define	_PACKET_H

#include "AddressPort.h"
#include "defines.h"
#include <string.h>
#include <assert.h>
#include "GarbageCollector.h"

using namespace std;

/**
 * Header wich holds virtual ports.
 * This will be inserted at the beginning of each Packet payload.
 */
struct wifu_end_header {
    u_int16_t virtual_src_port;
    u_int16_t virtual_dest_port;
};

#define END_HEADER_SIZE sizeof(struct wifu_end_header)

/**
 * Represents a packet with Source and Destination AddressPorts to help with a full-duplex connection.
 * The UDPInterface creates a Packet and inserts it into a UDPReceivePacketEvent which is then enqueued to the Dispatcher.
 * Protocols are to create Packet objects and insert them into UDPSendPacketEvents, then enqueue the Event to the Dispatcher.
 * The UDPInterface will then process the Event.
 *
 * @see UDPReceivePacketEvent
 * @see UDPSendPacketEvent
 * @see AddressPort
 * @see Protocol
 * @see Event
 * @see UDPInterface
 */
class Packet : public gc {
public:

    /**
     * Constructs a Packet object.
     * The internal data will first include the sport from source and the dport from dest (these ports are virtual).
     * The ports will be followed by data.
     *
     * @param source The source address and port of this Packet.
     * @param dest The destination address and port of this Packet.
     * @param data The payload of this Packet.
     * @param data_length The size of data in bytes.
     */
    Packet(AddressPort* source,
            AddressPort* dest,
            unsigned char* data,
            int data_length)
    : data_length_(data_length),
    source_(source),
    destination_(dest) {

        memset(payload_, 0, PAYLOAD_SIZE);

        struct wifu_end_header header;
        header.virtual_dest_port = dest->get_port();
        header.virtual_src_port = source->get_port();
        memcpy(payload_, &header, END_HEADER_SIZE);

        unsigned char* ptr = payload_;
        ptr += END_HEADER_SIZE;
        memcpy(ptr, data, data_length_);
    }

    /**
     * Converts this Packet to a byte array.
     * The returned value will have the two virtual ports followed by the data.
     *
     * @return The entire payload including virtual ports
     */
    virtual unsigned char* to_bytes() {
        return payload_;
    }

    /**
     * Returns the length of the data inside this Packet.
     *
     * @return The length of the data inside this Packet.
     */
    int data_length() {
        return data_length_;
    }

    /**
     * Returns the entire length of this Packet.
     * Namely, it returns data_lenth() + sizeof(wifu_end_header).
     *
     * @return data_lenth() + sizeof(wifu_end_header)
     *
     * @see Packet::data_length()
     * @see wifu_end_header
     */
    int packet_length() {
        return data_length() + END_HEADER_SIZE;
    }

    /**
     * Returns a pointer to the source AddressPort.
     * The address within is the actual source address.
     * The port within is the virtual port.
     *
     * @return A pointer to the source AddressPort.
     * @see AddressPort
     */
    AddressPort* get_source() {
        return source_;
    }

    /**
     * Returns a pointer to the destination AddressPort.
     * The address within is the actual destination address.
     * The port within is the virtual port.
     *
     * @return A pointer to the destination AddressPort.
     * @see AddressPort
     */
    AddressPort* get_destination() {
        return destination_;
    }

    /**
     * Returns a pointer to the data in this Packet.
     * This does NOT include the virtual ports.
     *
     * @return A pointer to the data in this Packet.
     */
    unsigned char* get_data() {
        return payload_ + END_HEADER_SIZE;
    }

private:

    /**
     * Contains the virtual ports and the data of this Packet.
     */
    unsigned char payload_[PAYLOAD_SIZE];

    /**
     * Size of the data within this Packet in bytes.
     */
    int data_length_;

    /**
     * Actual source address and virtual source port.
     */
    AddressPort* source_;

    /**
     * Actual destination address and virtual destination port.
     */
    AddressPort* destination_;
};

#endif	/* _PACKET_H */

