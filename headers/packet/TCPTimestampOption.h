/* 
 * File:   TCPTimeStampOption.h
 * Author: rbuck
 *
 * Created on April 27, 2011, 3:32 PM
 */

#ifndef TCPTIMESTAMPOPTION_H
#define	TCPTIMESTAMPOPTION_H

#include "TCPHeaderOption.h"
#include "Utils.h"
#include <netinet/tcp.h>
#include <netinet/in.h>

/**
 * TCP timestamp header option value.
 */
struct wifu_tcp_timestamp {
    /**
     * Timestamp value.
     */
    u_int32_t timestamp_value_;

    /**
     * Timestamp echo reply.
     */
    u_int32_t timestamp_echo_reply_;
};

/**
 * @see TCPHeaderOption
 */
class TCPTimestampOption : public TCPHeaderOption {
public:

    /**
     * Constructs a TCPTimestampOption
     * Sets the kind to TCPOPT_TIMESTAMP.
     * Sets the length to TCPOLEN_TIMESTAMP.
     *
     * @see netinet/tcp.h for details on the kind and length.
     * @see TCPHeaderOption::TCPHeaderOption
     */
    TCPTimestampOption();

    /**
     * Destructor.
     */
    ~TCPTimestampOption();

    /**
     * @return The timestamp value
     */
    u_int32_t get_timestamp();

    /**
     * Sets the option timestamp value to timestamp.
     * A default value for timestamp is provided (the current time in microseconds).
     *
     * @param timestamp The value to set as the option timestamp.
     */
    void set_timestamp(u_int32_t timestamp = Utils::get_current_time_microseconds_32());

    /**
     * @return The echo reply.
     */
    u_int32_t get_echo_reply();

    /**
     * Sets the option echo reply value to echo.
     * This should be the last received timestamp time.
     *
     * @param echo The value to set as the option echo reply.
     */
    void set_echo_reply(u_int32_t echo);

    /**
     * @return A pointer to a struct containing both the timestamp and echo reply.
     */
    struct wifu_tcp_timestamp* get_wifu_tcp_timestamp();

    /**
     * @return A string representation of this TCPTimestampOption object.
     */
    gcstring to_s();

};

#endif	/* TCPTIMESTAMPOPTION_H */

