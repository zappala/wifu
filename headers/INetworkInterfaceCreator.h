/* 
 * File:   NetworkInterfaceCreator.h
 * Author: rbuck
 *
 * Created on February 9, 2011, 3:52 PM
 */

#ifndef _NETWORKINTERFACECREATOR_H
#define	_NETWORKINTERFACECREATOR_H

#include "GarbageCollector.h"

class INetworkInterfaceCreator : public gc {
public:
    virtual INetworkInterface& create() = 0;
};

#endif	/* _NETWORKINTERFACECREATOR_H */

