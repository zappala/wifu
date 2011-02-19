#include "UDPReceivePacketEvent.h"

UDPReceivePacketEvent::UDPReceivePacketEvent(Socket* socket, WiFuPacket* packet) : PacketEvent(socket, packet) {}

UDPReceivePacketEvent::~UDPReceivePacketEvent() {}

void UDPReceivePacketEvent::execute(IModule* m) {
	m->udp_receive(this);
}
