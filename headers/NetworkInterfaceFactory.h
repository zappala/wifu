/* 
 * File:   NetworkInterfaceFactory.h
 * Author: rbuck
 *
 * Created on February 9, 2011, 3:49 PM
 */

#ifndef _NETWORKINTERFACEFACTORY_H
#define	_NETWORKINTERFACEFACTORY_H

#include "INetworkInterfaceCreator.h"

class NetworkInterfaceFactory {
public:
    static NetworkInterfaceFactory& instance() {
        static NetworkInterfaceFactory instance_;
        return instance_;
    }

    void set_creator(INetworkInterfaceCreator* creator) {
        creator_ = creator;
    }

    INetworkInterface& create() {
        return creator_->create();
    }

private:

    NetworkInterfaceFactory() {}

    INetworkInterfaceCreator* creator_;
};

#endif	/* _NETWORKINTERFACEFACTORY_H */

