/* 
 * File:   MockNetworkInterface.h
 * Author: rbuck
 *
 * Created on February 9, 2011, 4:25 PM
 */

#ifndef _MOCKNETWORKINTERFACE_H
#define	_MOCKNETWORKINTERFACE_H

#include "INetworkInterface.h"


class MockNetworkInterface : public INetworkInterface {
public:
    static MockNetworkInterface& instance() {
        static MockNetworkInterface instance_;
        return instance_;
    }

    virtual ~MockNetworkInterface() {

    }

    void start() {
        cout << "MockNetworkInterface::start()" << endl;
    }

    void register_protocol(int protocol, PacketFactory* pf) {
        cout << "MockNetworkInterface::register_protocol()" << endl;
    }

    void network_receive(WiFuPacket* p) {
        cout << "MockNetworkInterface::network_receive()" << endl;

    }

    void network_send(Event* e) {
        cout << "MockNetworkInterface::network_send()" << endl;
        NetworkSendPacketEvent* event = (NetworkSendPacketEvent*) e;


    }


private:
    MockNetworkInterface() : INetworkInterface() {
        
    }
};

#endif	/* _MOCKNETWORKINTERFACE_H */

