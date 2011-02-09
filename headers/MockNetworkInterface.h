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
        WiFuPacket* p = event->get_packet();

        AddressPort* remote = p->get_dest_address_port();
        AddressPort* local = p->get_source_address_port();

        Socket* s = SocketCollection::instance().get_by_local_and_remote_ap(local, remote);

        if (!s) {
            s = SocketCollection::instance().get_by_local_ap(local);
        }

        if (!s) {
            // No bound local socket
            return;
        }

        // TODO: create response packet
        WiFuPacket* response_packet = new TCPPacket();

        Event* response = new NetworkReceivePacketEvent(s, response_packet);
        Dispatcher::instance().enqueue(response);
    }


private:

    MockNetworkInterface() : INetworkInterface() {

    }


};

#endif	/* _MOCKNETWORKINTERFACE_H */

