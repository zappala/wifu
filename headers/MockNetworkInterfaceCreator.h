/* 
 * File:   MockNetworkInterfaceCreator.h
 * Author: rbuck
 *
 * Created on February 9, 2011, 4:30 PM
 */

#ifndef _MOCKNETWORKINTERFACECREATOR_H
#define	_MOCKNETWORKINTERFACECREATOR_H

#include "INetworkInterfaceCreator.h"
#include "MockNetworkInterface.h"

class MockNetworkInterfaceCreator : public INetworkInterfaceCreator {
public:
    INetworkInterface& create() {
        return MockNetworkInterface::instance();
    }
};

#endif	/* _MOCKNETWORKINTERFACECREATOR_H */

