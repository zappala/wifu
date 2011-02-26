/* 
 * File:   NetworkInterfaceCreator.h
 * Author: rbuck
 *
 * Created on February 9, 2011, 3:52 PM
 */

#ifndef _NETWORKINTERFACECREATOR_H
#define	_NETWORKINTERFACECREATOR_H

#include "GarbageCollector.h"

/**
 * Interface which forces the creation of an INetworkInterface.
 * This is part of the Mock Object pattern for the NetworkInterface
 *
 * @see INetworkInterface
 * @see NetworkInterface
 * @see MockNetworkInterface
 */
class INetworkInterfaceCreator : public gc {
public:

    /**
     * @return A reference to the INetworkInterface to be used during execution.
     */
    virtual INetworkInterface& create() = 0;
};

#endif	/* _NETWORKINTERFACECREATOR_H */

