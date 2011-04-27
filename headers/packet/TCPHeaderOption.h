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

#define TCP_HEADER_OPTION_MAX_SIZE 40

class TCPHeaderOption : public gc {
public:
    TCPHeaderOption(u_int8_t kind, u_int8_t length);
    ~TCPHeaderOption();
    u_int8_t* get_kind_pointer();
    u_int8_t* get_length_pointer();
    unsigned char* get_value_pointer();
    unsigned char* get_data();

private:
    u_int8_t* kind_;
    u_int8_t* length_;
    unsigned char* value_;
    unsigned char data_[TCP_HEADER_OPTION_MAX_SIZE];
};

#endif	/* TCPHEADEROPTION_H */

