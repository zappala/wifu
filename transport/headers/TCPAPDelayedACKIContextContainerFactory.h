/* 
 * File:   TCPAPDelayedACKIContextContainerFactory.h
 * Author: leer
 *
 * Created on July 6, 2011, 2:03 PM
 */

#ifndef _TCPAPDELAYEDACKICONTEXTCONTAINERFACTORY_H
#define	_TCPAPDELAYEDACKICONTEXTCONTAINERFACTORY_H

#include "IContextContainerFactory.h"
#include "contexts/TCPAPDelayedACKIContextContainer.h"

class TCPAPDelayedACKIContextContainerFactory : public IContextContainerFactory{
public:
    BasicIContextContainer* get_icontext_container();
private:

};

#endif	/* _TCPAPDELAYEDACKICONTEXTCONTAINERFACTORY_H */

