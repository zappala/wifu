#include "events/UDPSendPacketEvent.h"

UDPSendPacketEvent::UDPSendPacketEvent(Socket* socket, WiFuPacket* packet) : PacketEvent(socket, packet) {}

UDPSendPacketEvent::~UDPSendPacketEvent() {}

void UDPSendPacketEvent::execute(IModule* m) {
	m->udp_send(this);
}
