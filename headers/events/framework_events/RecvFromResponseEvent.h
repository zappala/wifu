/* 
 * File:   RecvFromResponseEvent.h
 * Author: rbuck
 *
 * Created on August 11, 2011, 11:19 AM
 */

#ifndef RECVFROMRESPONSEEVENT_H
#define	RECVFROMRESPONSEEVENT_H

#include "AddressResponseEvent.h"

class RecvFromResponseEvent : public AddressResponseEvent {
public:
    RecvFromResponseEvent();
    ~RecvFromResponseEvent();

    /**
     * This must be called AFTER the address has been set (we recalculate the message length)
     * @param buffer
     * @param length
     */
    void set_return_buffer(unsigned char* buffer, size_t length);
};

#endif	/* RECVFROMRESPONSEEVENT_H */

