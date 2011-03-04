#include "NetworkInterface.h"

NetworkInterface& NetworkInterface::instance() {
	static NetworkInterface instance_;
	return instance_;
}

NetworkInterface::~NetworkInterface() {}

void NetworkInterface::start() {
	listener_.start(this);
}

void NetworkInterface::register_protocol(int protocol, PacketFactory* pf) {
	listener_.register_protocol(protocol, pf);
}

void NetworkInterface::network_receive(WiFuPacket* p) {
	AddressPort* remote = p->get_source_address_port();
	AddressPort* local = p->get_dest_address_port();

	Socket* s = SocketCollection::instance().get_by_local_and_remote_ap(local, remote);

	if (!s) {
		s = SocketCollection::instance().get_by_local_ap(local);
	}

	if (!s) {
		// No bound local socket
		//TODO: should it really just return like this or should it throw an exception? -Scott
		return;
	}

        cout << "NetworkInterface::network_receive(), socket: " << s << endl;
	Event* e = new NetworkReceivePacketEvent(s, p);
	Dispatcher::instance().enqueue(e);
}

void NetworkInterface::network_send(Event* e) {
	NetworkSendPacketEvent* event = (NetworkSendPacketEvent*) e;
	// TODO: Check return value (bytes sent)?
        cout << "NetworkInterface::network_send()" << endl;
	sender_.send(event->get_packet());
}

NetworkInterface::NetworkInterface() : INetworkInterface() {

}
