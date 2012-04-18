/* 
 * File:   TCPAPIContextContainerFactory.h
 * Author: leer
 *
 * Created on July 5, 2011, 2:54 PM
 */

#ifndef _TCPAPICONTEXTCONTAINERFACTORY_H
#define	_TCPAPICONTEXTCONTAINERFACTORY_H

#include "IContextContainerFactory.h"
#include "contexts/TCPAPIContextContainer.h"

class TCPAPIContextContainerFactory : public IContextContainerFactory {
public:
    virtual BasicIContextContainer* get_icontext_container();
};

#endif	/* _TCPAPICONTEXTCONTAINERFACTORY_H */

