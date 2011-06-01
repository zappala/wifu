#include "events/framework_events/UDPReceivePacketEvent.h"

UDPReceivePacketEvent::UDPReceivePacketEvent(Socket* socket, WiFuPacket* packet) : PacketHolder(packet), FrameworkEvent(socket) {}

UDPReceivePacketEvent::~UDPReceivePacketEvent() {}

void UDPReceivePacketEvent::execute(IModule* m) {
	m->imodule_udp_receive(this);
}
