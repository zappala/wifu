#include "MockNetworkInterface.h"

MockNetworkInterface& MockNetworkInterface::instance() {
    static MockNetworkInterface instance_;
    return instance_;
}

MockNetworkInterface::~MockNetworkInterface() {
}

void MockNetworkInterface::start() {
}

void MockNetworkInterface::register_protocol(int protocol, PacketFactory* pf) {
}

void MockNetworkInterface::network_receive(WiFuPacket* p) {
}

void MockNetworkInterface::network_send(Event* e) {
    //Can use counter_ to specify which packets drop
    if (counter_++ == 0) {
        cout << "Dropping the initial SYN.\n";
        return;
    }
    else if (counter_ == 2) {
        cout << "Dropping the SYNACK.\n";
        return;
    }
    else if (counter_ == 4) {
        cout << "Dropping the ACK of the SYNACK and going for a SNACK.\n";
        return;
    }
    //TODO: make sure we're dropping the packets we think we're dropping (and thus say)
    assert(false);
    NetworkSendPacketEvent* event = (NetworkSendPacketEvent*) e;
    WiFuPacket* p = event->get_packet();

    AddressPort* local = p->get_dest_address_port();
    AddressPort* remote = p->get_source_address_port();

    Socket* s = SocketCollection::instance().get_by_local_and_remote_ap(local, remote);

    if (!s) {
        s = SocketCollection::instance().get_by_local_ap(local);
    }

    if (!s) {
        // No bound local socket
        return;
    }

    Event* response = new NetworkReceivePacketEvent(s, p);
    Dispatcher::instance().enqueue(response);
}

MockNetworkInterface::MockNetworkInterface() : INetworkInterface() {
    counter_ = 0;
}
