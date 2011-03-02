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
    NetworkSendPacketEvent* event = (NetworkSendPacketEvent*) e;
    WiFuPacket* p = event->get_packet();
    TCPPacket* tcp_packet = (TCPPacket*) p;

    if (should_drop(tcp_packet)) {
        return;
    }

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
    syn_ = synack_ = ack_ = false;
}

// Will drop each leg of three-way handshake only once

bool MockNetworkInterface::should_drop(TCPPacket* p) {
    //return should_drop_syn(p) || should_drop_synack(p) || should_drop_ack(p);
    return should_drop_syn(p) || should_drop_synack(p);
//        return should_drop_synack(p);
//    return should_drop_ack(p);
            //return should_drop_syn(p);
    //    return false;
}

bool MockNetworkInterface::should_drop_syn(TCPPacket* p) {
    if (syn_) {
        return false;
    }
    syn_ = p->is_tcp_syn() && !p->is_tcp_ack();
    if (syn_) {
        cout << "Dropping the initial SYN.\n";
    }
    return syn_;
}

bool MockNetworkInterface::should_drop_synack(TCPPacket* p) {
    if (synack_) {
        return false;
    }
    synack_ = p->is_tcp_syn() && p->is_tcp_ack();
    if (synack_) {
        cout << "Dropping the SYNACK.\n";
    }
    return synack_;
}

bool MockNetworkInterface::should_drop_ack(TCPPacket* p) {
    if (ack_) {
        return false;
    }
    ack_ = !p->is_tcp_syn() && p->is_tcp_ack();
    if (ack_) {
        cout << "Dropping the ACK of the SYNACK and going for a SNACK.\n";
    }
    return ack_;
}
