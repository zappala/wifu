/* 
 * File:   DummyIContextContainerFactory.h
 * Author: rbuck
 *
 * Created on July 19, 2011, 12:07 PM
 */

#ifndef DUMMYICONTEXTCONTAINERFACTORY_H
#define	DUMMYICONTEXTCONTAINERFACTORY_H

#include "TCPTahoeIContextContainerFactory.h"
#include "DummyProtocolIContextContainer.h"


class DummyIContextContainerFactory : public TCPTahoeIContextContainerFactory {
public:
    BasicIContextContainer* get_icontext_container();
};

#endif	/* DUMMYICONTEXTCONTAINERFACTORY_H */

