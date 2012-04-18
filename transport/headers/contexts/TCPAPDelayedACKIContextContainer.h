/* 
 * File:   TCPAPDelayedACKIContextContainer.h
 * Author: leer
 *
 * Created on July 6, 2011, 2:08 PM
 */

#ifndef _TCPAPDELAYEDACKICONTEXTCONTAINER_H
#define	_TCPAPDELAYEDACKICONTEXTCONTAINER_H

#include "contexts/TCPDelayedACKReliabilityContext.h"
#include "contexts/TCPAPIContextContainer.h"

class TCPAPDelayedACKIContextContainer : public TCPAPIContextContainer{
public:
    TCPAPDelayedACKIContextContainer();
    virtual ~TCPAPDelayedACKIContextContainer();
private:

};

#endif	/* _TCPAPDELAYEDACKICONTEXTCONTAINER_H */

