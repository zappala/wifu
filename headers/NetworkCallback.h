/* 
 * File:   NetworkCallback.h
 * Author: rbuck
 *
 * Created on January 28, 2011, 11:51 AM
 */

#ifndef _NETWORKCALLBACK_H
#define	_NETWORKCALLBACK_H

#include "packet/WiFuPacket.h"

class NetworkCallback {
public:
    /**
     * Callback function to be called when a packet is received from the network.
     *
     */
    virtual void receive(WiFuPacket* p) = 0;
};

#endif	/* _NETWORKCALLBACK_H */

