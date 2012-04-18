/* 
 * File:   TCPDelayedACKIContextContainerFactory.h
 * Author: leer
 *
 * Created on June 22, 2011, 8:39 AM
 */

#ifndef TCPDELAYEDACKICONTEXTCONTAINERFACTORY_H
#define	TCPDELAYEDACKICONTEXTCONTAINERFACTORY_H

#include "IContextContainerFactory.h"
#include "contexts/TCPDelayedACKIContextContainer.h"

class TCPDelayedACKIContextContainerFactory : public IContextContainerFactory {
public:
    virtual BasicIContextContainer* get_icontext_container();
};

#endif	/* TCPDELAYEDACKICONTEXTCONTAINERFACTORY_H */

