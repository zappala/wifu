/* 
 * File:   IPortManagerCreator.h
 * Author: rbuck
 *
 * Created on April 11, 2011, 5:03 PM
 */

#ifndef IPORTMANAGERCREATOR_H
#define	IPORTMANAGERCREATOR_H

#include "GarbageCollector.h"

class IPortManagerCreator : public gc {
public:
    virtual IPortManager& create() = 0;
};

#endif	/* IPORTMANAGERCREATOR_H */

