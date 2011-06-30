/* 
 * File:   TCPTahoe_DelayedACKs.h
 * Author: rbuck/leer
 *
 * Created on May 4, 2011, 2:44 PM
 */

#ifndef TCPTAHOE_DELAYEDACKS_H
#define	TCPTAHOE_DELAYEDACKS_H

#include "TCPTahoe.h"
#include "IContextContainerFactory.h"
#include "TCPDelayedACKIContextContainerFactory.h"
#include "contexts/TCPDelayedACKIContextContainer.h"

class TCPDelayedACK : public TCPTahoe {
protected:
	TCPDelayedACK(int protocol = TCP_DELAYEDACK, IContextContainerFactory* factory = new TCPDelayedACKIContextContainerFactory());

public:
    static TCPDelayedACK& instance();
    virtual ~TCPDelayedACK();
};

#endif	/* TCPTAHOE_DELAYEDACKS_H */

