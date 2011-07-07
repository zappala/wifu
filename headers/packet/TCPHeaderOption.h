/* 
 * File:   TCPHeaderOption.h
 * Author: rbuck
 *
 * Created on April 27, 2011, 3:03 PM
 */

#ifndef TCPHEADEROPTION_H
#define	TCPHEADEROPTION_H

#include "GarbageCollector.h"
# include <sys/types.h>
#include <string.h>
#include <assert.h>

#define TCP_HEADER_OPTION_MAX_SIZE 40

/**
 * Base TCP header option class.
 * Provides simple functionallity for dealing with TCP options.
 * It is intended that this class will be inherited from.
 * All values that are stored which are 16-bits or longer need to be stored in network byte order.
 * I recommend doing it like TCPPacket, where values are stored in network byte order, but the interface is in host byte order.
 *
 * All kind and length varaibles SHOULD be one of the #defines in netinet/tcp.h.
 * For example the TCP timestamp option's kind and length are TCPOPT_TIMESTAMP and TCPOLEN_TIMESTAMP respectively.
 *
 * @see TCPPacket
 */
class TCPHeaderOption : public gc {
public:

    /**
     * Constructor.
     * @param kind Kind of this TCP header option.
     * @param length Length in bytes of this TCP header option.
     */
    TCPHeaderOption(u_int8_t kind, u_int8_t length);

    /**
     * Distructor.
     */
    virtual ~TCPHeaderOption();
    
    /**
     * @return The kind of this TCP header option.
     */
    u_int8_t get_kind();

    /**
     * Sets the kind of this TCP header option.
     * @param kind The kind to set.
     */
    void set_kind(u_int8_t kind);

    /**
     * @return The length in bytes of this TCP header option.
     */
    u_int8_t get_length();

    /**
     * Sets the length of this TCP header option.
     * @param length The length to set.
     * @pre length <= TCP_HEADER_OPTION_MAX_SIZE
     */
    void set_length(u_int8_t length);

    /**
     * @return A pointer to the value portion of the option (after the kind and length)
     */
    unsigned char* get_value();

    /**
     * Copies the value_length bytes from the buffer pointed to by value into this TCPHeaderOption's value area (memory after the kind and length).
     * @param value Pointer to a buffer containing the option value.
     * @param value_length Length of data pointed to by value in bytes.
     *
     * @pre 2 + value_length <= TCP_HEADER_OPTION_MAX_SIZE
     * @pre 2 + value_length == TCPHeaderOption::get_length()
     */
    void set_value(unsigned char* value, u_int8_t value_length);

    /**
     * @return A pointer to the beginning of the option.
     */
    unsigned char* get_data();

    /**
     * Sets this TCPHeaderOptions kind, length, and value all in one shot.
     * @param data Pointer to the beginning of an option.
     * @param data_length Length of the buffer pointed to by data.
     */
    void set_data(unsigned char* data, u_int8_t data_length);

private:

    /**
     * Buffer to hold option information.
     */
    unsigned char data_[TCP_HEADER_OPTION_MAX_SIZE];
};

#endif	/* TCPHEADEROPTION_H */

