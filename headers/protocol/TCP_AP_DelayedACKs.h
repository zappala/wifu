/* 
 * File:   TCP_AP_DelayedACKs.h
 * Author: leer
 *
 * Created on July 6, 2011, 2:36 PM
 */

#ifndef _TCP_AP_DELAYEDACKS_H
#define	_TCP_AP_DELAYEDACKS_H

#include "defines.h"
#include "protocol/TCPAP.h"
#include "TCPAPDelayedACKIContextContainerFactory.h"

class TCP_AP_DelayedACKs : public TCPAP{
public:
    TCP_AP_DelayedACKs(int protocol, IContextContainerFactory* factory);
    virtual ~TCP_AP_DelayedACKs();

    static TCP_AP_DelayedACKs& instance();
private:

};

#endif	/* _TCP_AP_DELAYEDACKS_H */

