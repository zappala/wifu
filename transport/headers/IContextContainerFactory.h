/* 
 * File:   IContextContainerFactory.h
 * Author: rbuck
 *
 * Created on June 28, 2011, 4:58 PM
 */

#ifndef ICONTEXTCONTAINERFACTORY_H
#define	ICONTEXTCONTAINERFACTORY_H

#include "GarbageCollector.h"
#include "contexts/BasicIContextContainer.h"

class IContextContainerFactory : public gc {
public:
    virtual BasicIContextContainer* get_icontext_container() = 0;
};

#endif	/* ICONTEXTCONTAINERFACTORY_H */

