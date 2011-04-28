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
    
    
    u_int8_t get_kind();

    void set_kind(u_int8_t kind);

    u_int8_t get_length();

    void set_length(u_int8_t length);

    /**
     *
     * @return A pointer to the value portion of the option (after the kind and length)
     */
    unsigned char* get_value();

    /**
     * 
     * @return A pointer to the beginning of all the data
     */
    unsigned char* get_data();

private:
    unsigned char data_[TCP_HEADER_OPTION_MAX_SIZE];
};

#endif	/* TCPHEADEROPTION_H */

