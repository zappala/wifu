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

    u_int32_t* get_timestamp_pointer();
    u_int32_t* get_echo_pointer();
    struct timestamp* get_timestamp_struct_pointer();
    
    void set_echo_value(u_int32_t echo);
    void set_timestamp(u_int32_t timestamp = Utils::get_current_time_microseconds_32());
    

private:
    struct timestamp* timestamp_;

    u_int32_t get_current_time();
};

#endif	/* TCPTIMESTAMPOPTION_H */

