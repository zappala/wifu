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

// TODO: do we need to do anything with byte ordering in this or any derived classes?
// TODO: Yes, all items that are stored which are bigger than 16 bits will need to be saved in network byte order.

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

    void set_value(unsigned char* value, u_int8_t value_length);

    /**
     * 
     * @return A pointer to the beginning of all the data
     */
    unsigned char* get_data();

    void set_data(unsigned char* data, u_int8_t data_length);

private:
    unsigned char data_[TCP_HEADER_OPTION_MAX_SIZE];
};

#endif	/* TCPHEADEROPTION_H */

