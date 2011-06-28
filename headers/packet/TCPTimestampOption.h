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
#include <sstream>

struct wifu_tcp_timestamp {
    u_int32_t timestamp_value_;
    u_int32_t timestamp_echo_reply_;
};

class TCPTimestampOption : public TCPHeaderOption {
public:
    TCPTimestampOption();
    ~TCPTimestampOption();

    u_int32_t get_timestamp();

    void set_timestamp(u_int32_t timestamp = Utils::get_current_time_microseconds_32());

    u_int32_t get_echo_reply();

    void set_echo_reply(u_int32_t echo);

    struct wifu_tcp_timestamp* get_wifu_tcp_timestamp();

    string to_s();

};

#endif	/* TCPTIMESTAMPOPTION_H */

