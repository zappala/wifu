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
    virtual void receive(WiFuPacket* p) = 0;
};

#endif	/* _NETWORKCALLBACK_H */

