#include "events/framework_events/UDPSendPacketEvent.h"

UDPSendPacketEvent::UDPSendPacketEvent(Socket* socket, WiFuPacket* packet) : PacketEvent(socket, packet) {}

UDPSendPacketEvent::~UDPSendPacketEvent() {}

void UDPSendPacketEvent::execute(IModule* m) {
	m->imodule_udp_send(this);
}
