/*
 * ATPPacket.h
 *
 *  Created on: Jun 10, 2011
 *      Author: philipbl
 */

#ifndef ATPPACKET_H_
#define ATPPACKET_H_

#include "packet/TCPPacket.h"

class ATPPacket: public TCPPacket {

public:

	ATPPacket();

	~ATPPacket();

	void init();

	/**
	 * Gets the data from the packet.
	 */
	virtual unsigned char* get_data();
	virtual int get_data_length_bytes();

	/**
	 * Sets the data of the packet.
	 */
	virtual void set_data(unsigned char* data, int length);

	/**
	 * Not sure if I need this to be overwritten. It just calls the super class.
	 */
	virtual void pack();

	/**
	 * Returns pointer to the start of the atp header.
	 */
	unsigned char* get_atp_header();

	/**
	 * Gets the max delay from the packet. This delay is inserted into
	 * the header of the packet at intermediate nodes (thanks to WiFu).
	 */
	u_int32_t get_atp_max_delay();

	/**
	 * Sets the max delay of the packet. I don't think I should have
	 * to use this.
	 */
	void set_atp_max_delay(u_int32_t delay);

	/**
	 * Gets the average delay of the packet. This is calculated at the
	 * receiving end and sent in an ACK.
	 */
	u_int32_t get_atp_average_delay();

	/**
	 * Sets the average delay of the packet. This should only be set if
	 * node is receiving data. The delay is sent in an ACK.
	 */
	void set_atp_average_delay(u_int32_t delay);

	/**
	 * Returns the length of the atp header in bytes. Should stay consistent.
	 */
    int get_atp_header_length_bytes() const;

    /**
     * Returns the max amount of data that can be stored in the packet once you
     * subtract all the room the headers are taking up.
     */
    virtual int max_data_length();


    virtual string to_s() const;
    virtual string to_s_format() const;

    virtual bool operator ==(const IPPacket& other) const;
    virtual bool operator !=(const IPPacket& other) const;

    /**
     * This is used to tell the atp header to move down since another option
     * is being added. It then calls the parent class method.
     */
    void insert_tcp_header_option(TCPHeaderOption* option);

    /**
     * This is used to tell the atp header to move up since an option
     * was deleted. It then calls the parent class method.
     */
    TCPHeaderOption* remove_tcp_header_option(u_int8_t kind);

    static ATPPacket * convert_to_atp_packet(WiFuPacket * wp);

    /**
     * Updates where the header pointer points to.
     */
    void update_header();

private:
    ATPPacket(TCPPacket* p);
    /**
     * Moves the atp header to a new location in memory based on what
     * tcp_data_offset is set to.
     */
    void move_atp_header();

    /** The atp structure data structure */
	struct atp_packet_header {
		u_int32_t max_delay;
		u_int32_t average_delay;
	};

	/** Reference to parent class */
	typedef TCPPacket super;

	/** Pointer to memory of packet */
	atp_packet_header* atp_;

	/** Makes sure data can't be set twice */
	bool data_set_;
};

#endif /* ATPPACKET_H_ */
