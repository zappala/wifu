/* 
 * File:   TCPFewIContextContainerFactory.h
 * Author: leer
 *
 * Created on June 22, 2011, 8:39 AM
 */

#ifndef TCPFEWICONTEXTCONTAINERFACTORY_H
#define	TCPFEWICONTEXTCONTAINERFACTORY_H

#include "IContextContainerFactory.h"
#include "contexts/TCPFewIContextContainer.h"

class TCPFewIContextContainerFactory : public IContextContainerFactory {
public:
    virtual BasicIContextContainer* get_icontext_container();
};

#endif	/* TCPFEWICONTEXTCONTAINERFACTORY_H */

