/* 
 * File:   INetworkInterface.h
 * Author: rbuck
 *
 * Created on February 9, 2011, 3:54 PM
 */

#ifndef _INETWORKINTERFACE_H
#define	_INETWORKINTERFACE_H

#include "PacketFactory.h"
#include "Module.h"
#include "NetworkCallback.h"

class INetworkInterface : public Module, public NetworkCallback {
public:
    INetworkInterface() : Module(), NetworkCallback() {}
    
    virtual void start() = 0;
    virtual void register_protocol(int protocol, PacketFactory* pf) = 0;
    virtual void network_send(Event* e) = 0;
};

#endif	/* _INETWORKINTERFACE_H */

