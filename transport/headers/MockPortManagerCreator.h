/* 
 * File:   MockPortManagerCreator.h
 * Author: rbuck
 *
 * Created on April 11, 2011, 5:41 PM
 */

#ifndef MOCKPORTMANAGERCREATOR_H
#define	MOCKPORTMANAGERCREATOR_H

#include "IPortManagerCreator.h"
#include "MockPortManager.h"


class MockPortManagerCreator : public IPortManagerCreator {
public:
    IPortManager& create() {
        return MockPortManager::instance();
    }
};

#endif	/* MOCKPORTMANAGERCREATOR_H */

