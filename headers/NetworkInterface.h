/* 
 * File:   NetworkInterface.h
 * Author: rbuck
 *
 * Created on January 28, 2011, 11:50 AM
 */

#ifndef _NETWORKINTERFACE_H
#define	_NETWORKINTERFACE_H

#include "events/framework_events/NetworkSendPacketEvent.h"
#include "events/framework_events/NetworkReceivePacketEvent.h"
#include "RawSocketSender.h"
#include "RawSocketListener.h"
#include "AddressPort.h"
#include "SocketCollection.h"
#include "INetworkInterface.h"

class NetworkInterface : public INetworkInterface {
public:

    static NetworkInterface& instance();

    virtual ~NetworkInterface();

    void start();

    void register_protocol(int, PacketFactory*);

    void imodule_network_receive(WiFuPacket*);

    void imodule_network_send(Event*);

private:

    NetworkInterface();

    int socket_;
    map<int, PacketFactory*, std::less<int>, gc_allocator<std::pair<int, PacketFactory*> > > protocols_;
    RawSocketSender sender_;
    RawSocketListener listener_;
};

#endif	/* _NETWORKINTERFACE_H */

