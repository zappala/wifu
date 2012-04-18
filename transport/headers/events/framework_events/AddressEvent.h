/* 
 * File:   AddressEvent.h
 * Author: rbuck
 *
 * Created on August 11, 2011, 10:09 AM
 */

#ifndef ADDRESSEVENT_H
#define	ADDRESSEVENT_H

#include "LibraryEvent.h"
#include "AddressPort.h"

using namespace std;

class AddressEvent : public LibraryEvent {
public:
    AddressEvent();
    virtual ~AddressEvent();

    /**
     * @return The address and port passed into the socket call.
     */
    AddressPort* get_address();

private:
    AddressPort* address_;
};

#endif	/* ADDRESSEVENT_H */

