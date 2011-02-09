/* 
 * File:   StandardNetworkInterfaceCreator.h
 * Author: rbuck
 *
 * Created on February 9, 2011, 4:03 PM
 */

#ifndef _STANDARDNETWORKINTERFACECREATOR_H
#define	_STANDARDNETWORKINTERFACECREATOR_H

#include "INetworkInterfaceCreator.h"
#include "NetworkInterface.h"

class StandardNetworkInterfaceCreator : public INetworkInterfaceCreator {
public:
    INetworkInterface& create() {
        return NetworkInterface::instance();
    }
};

#endif	/* _STANDARDNETWORKINTERFACECREATOR_H */

