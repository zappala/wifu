/* 
 * File:   StandardPortManagerCreator.h
 * Author: rbuck
 *
 * Created on April 11, 2011, 5:08 PM
 */

#ifndef STANDARDPORTMANAGERCREATOR_H
#define	STANDARDPORTMANAGERCREATOR_H

#include "IPortManagerCreator.h"
#include "PortManager.h"


class StandardPortManagerCreator : public IPortManagerCreator {
public:
    IPortManager& create() {
        return PortManager::instance();
    }
};

#endif	/* STANDARDPORTMANAGERCREATOR_H */

