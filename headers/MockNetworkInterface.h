/* 
 * File:   MockNetworkInterface.h
 * Author: rbuck
 *
 * Created on February 9, 2011, 4:25 PM
 */

#ifndef _MOCKNETWORKINTERFACE_H
#define	_MOCKNETWORKINTERFACE_H

#include "INetworkInterface.h"
#include "events/NetworkReceivePacketEvent.h"
#include "packet/TCPPacket.h"

class MockNetworkInterface : public INetworkInterface {
public:
    static MockNetworkInterface& instance();

    virtual ~MockNetworkInterface();

    void start();

    void register_protocol(int protocol, PacketFactory* pf);

    void network_receive(WiFuPacket* p);

    void network_send(Event* e);

private:
    MockNetworkInterface();

    bool should_drop(TCPPacket* p);
    bool should_drop_syn(TCPPacket* p);
    bool should_drop_synack(TCPPacket* p);
    bool should_drop_ack(TCPPacket* p);
    int counter_;
    bool syn_;
    bool synack_;
    bool ack_;
};

#endif	/* _MOCKNETWORKINTERFACE_H */

