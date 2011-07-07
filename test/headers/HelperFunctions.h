/* 
 * File:   HelperFunctions.h
 * Author: rbuck
 *
 * Created on May 16, 2011, 3:33 PM
 */

#ifndef HELPERFUNCTIONS_H
#define	HELPERFUNCTIONS_H

#include "defines.h"
#include "packet/TCPPacket.h"

#include <sys/types.h>

using namespace std;

class HelperFunctions {
public:
    static TCPPacket* get_tcp_packet_with_data(u_int32_t sequence_number, gcstring& data) {
        TCPPacket* p = new TCPPacket();
        p->set_tcp_sequence_number(sequence_number);
        p->set_data((unsigned char*) data.data(), data.size());
        return p;
    }
};

#endif	/* HELPERFUNCTIONS_H */

