#include "UDPInterface.h"

UDPInterface& UDPInterface::instance() {
	static UDPInterface instance_;
	return instance_;
}

void UDPInterface::start(AddressPort& ap) {
	// Set up the UDPSocket
	socket_.bind_socket(ap);
	socket_.receive(this);
}

UDPInterface::~UDPInterface() {}

void UDPInterface::receive(AddressPort& ap, unsigned char* buffer, size_t length) {
	WiFuPacket* p = new WiFuPacket(buffer, length);
	AddressPort* remote = p->get_source_address_port();
	AddressPort* local = p->get_dest_address_port();

	Socket* s = SocketCollection::instance().get_by_local_and_remote_ap(local, remote);

	if(!s) {
		s = SocketCollection::instance().get_by_local_ap(local);
	}

	if(!s) {
		// No bound local socket
		return;
	}

	Event* e = new UDPReceivePacketEvent(s, p);
	Dispatcher::instance().enqueue(e);
}

void UDPInterface::udp_send(Event* e) {
	UDPSendPacketEvent* event = (UDPSendPacketEvent*) e;
	WiFuPacket* p = event->get_packet();
	string dest = p->get_ip_destination_address_s();
	AddressPort destination(dest, WIFU_PORT);
	socket_.send(destination, p->get_payload(), p->get_ip_datagram_length());
}

string& UDPInterface::get_bound_ip_address() {
	return socket_.get_bound_address_port()->get_address();
}

UDPInterface::UDPInterface() : Module(), UDPSocketCallback() {}
