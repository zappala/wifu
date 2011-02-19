/* 
 * File:   UDPSocketCallback.h
 * Author: rbuck
 *
 * Created on June 15, 2010, 4:15 PM
 */

#ifndef _UDPSOCKETCALLBACK_H
#define	_UDPSOCKETCALLBACK_H

#include <iostream>
#include "AddressPort.h"

using namespace std;

class UDPSocketCallback{
public:
    UDPSocketCallback () {}

    virtual ~UDPSocketCallback() {}

    virtual void receive(AddressPort& ap, unsigned char* buffer, size_t length) = 0;
};


#endif	/* _UDPSOCKETCALLBACK_H */

