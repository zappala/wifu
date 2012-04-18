/* 
 * File:   AddressResponseEvent.h
 * Author: rbuck
 *
 * Created on August 11, 2011, 1:06 PM
 */

#ifndef ADDRESSRESPONSEEVENT_H
#define	ADDRESSRESPONSEEVENT_H

#include "ResponseEvent.h"

class AddressResponseEvent : public ResponseEvent {
public:
    AddressResponseEvent();
    virtual ~AddressResponseEvent();

    void set_addr(struct sockaddr_in* addr, socklen_t addr_len);
};

#endif	/* ADDRESSRESPONSEEVENT_H */

