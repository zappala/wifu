/*
 * ATPIContextContainerFactory.cc
 *
 *  Created on: Jun 29, 2011
 *      Author: philipbl
 */

#include "ATPIContextContainerFactory.h"

BasicIContextContainer* ATPIContextContainerFactory::get_icontext_container() {
    return new ATPIContextContainer();
}
