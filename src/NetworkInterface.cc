#include "NetworkInterface.h"

NetworkInterface& NetworkInterface::instance() {
    static NetworkInterface instance_;
    return instance_;
}

NetworkInterface::~NetworkInterface() {
}

void NetworkInterface::start() {
    listener_.start(this);
}

void NetworkInterface::register_protocol(int protocol, PacketFactory* pf) {
    listener_.register_protocol(protocol, pf);
}

void NetworkInterface::imodule_network_receive(WiFuPacket* p) {
    AddressPort* remote = p->get_source_address_port();
    AddressPort* local = p->get_dest_address_port();

    //cout << "Network receiving packet: " << p->to_s() << endl;
    Socket* s = SocketCollection::instance().get_by_local_and_remote_ap(local, remote);

    if (!s) {
        //cout << "Trying to find socket...\n";
        s = SocketCollection::instance().get_by_local_ap(local);
    }

    if (!s) {
        // No bound local socket
        //TODO: should it really just return like this or should it throw an exception? -Scott
        //cout << "Dropping packet, we didn't find a valid socket...\n";
        return;
    }

    Event* e = new NetworkReceivePacketEvent(s, p);
    //cout << "NetworkInterface::imodule_network_receive: dispatching NetworkReceivePacketEvent.\n";
    Dispatcher::instance().enqueue(e);
}

void NetworkInterface::imodule_network_send(Event* e) {
    NetworkSendPacketEvent* event = (NetworkSendPacketEvent*) e;
    // TODO: Check return value (bytes sent)?
    sender_.send(event->get_packet());
}

NetworkInterface::NetworkInterface() : INetworkInterface() {

}
