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

struct timestamp {
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

    struct timestamp* get_timestamp_struct_pointer();

private:
    struct timestamp* timestamp_;
};

#endif	/* TCPTIMESTAMPOPTION_H */

