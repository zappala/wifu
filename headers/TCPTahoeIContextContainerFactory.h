/* 
 * File:   TCPTahoeIContextContainerFactory.h
 * Author: rbuck
 *
 * Created on June 28, 2011, 5:00 PM
 */

#ifndef TCPTAHOEICONTEXTCONTAINERFACTORY_H
#define	TCPTAHOEICONTEXTCONTAINERFACTORY_H

#include "IContextContainerFactory.h"
#include "contexts/TCPTahoeIContextContainer.h"

class TCPTahoeIContextContainerFactory : public IContextContainerFactory {
public:
    virtual BasicIContextContainer* get_icontext_container();
};

#endif	/* TCPTAHOEICONTEXTCONTAINERFACTORY_H */

