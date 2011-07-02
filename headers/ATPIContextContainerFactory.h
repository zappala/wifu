/*
 * ATPIContextContainerFactory.h
 *
 *  Created on: Jun 29, 2011
 *      Author: philipbl
 */

#ifndef ATPICONTEXTCONTAINERFACTORY_H_
#define ATPICONTEXTCONTAINERFACTORY_H_

#include "IContextContainerFactory.h"
#include "contexts/ATPIContextContainer.h"

class ATPIContextContainerFactory : public IContextContainerFactory {
public:
    virtual BasicIContextContainer* get_icontext_container();
};

#endif /* ATPICONTEXTCONTAINERFACTORY_H_ */
