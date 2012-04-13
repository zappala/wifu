/* 
 * File:   TCPDelayedACKIContextContainer.h
 * Author: rbuck/leer
 *
 * Created on May 4, 2011, 3:55 PM
 */

#ifndef TCPDELAYEDACKICONTEXTCONTAINER_H
#define	TCPDELAYEDACKICONTEXTCONTAINER_H

#include "contexts/TCPTahoeIContextContainer.h"
#include "contexts/TCPDelayedACKReliabilityContext.h"

class TCPDelayedACKIContextContainer : public TCPTahoeIContextContainer {
public:
    TCPDelayedACKIContextContainer();
    virtual ~TCPDelayedACKIContextContainer();
};

#endif	/* TCPDELAYEDACKICONTEXTCONTAINER_H */

